#include <algorithm>
#include <future>
#include <cassert>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <fmt/printf.h>
#include <fmt/format.h>

#include <xcord/discord.hpp>
#include <xcord/websocket.hpp>

#undef GetObject

namespace xcord
{
	Websocket::Websocket()
	{
		client_.clear_access_channels(websocketpp::log::alevel::all);
		client_.init_asio();

		client_.set_tls_init_handler([this](const websocketpp::connection_hdl) -> auto {
			return std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv13);
		});

		client_.set_open_handler([this](const websocketpp::connection_hdl hdl) -> auto {
			connected_ = true;
			handle_ = hdl;
		});

		client_.set_message_handler([this](const websocketpp::connection_hdl, const message_ptr message) -> auto {
			std::async(std::launch::async, [this, &message]() -> void {
				auto inflated = inflator_.inflate(message->get_payload());

				if (inflated)
				{
					rapidjson::Document document;
					document.Parse(inflated.value().data());

					if (document.IsObject())
					{
						if (sequence_ == -1)
						{
							if (document.HasMember("s"))
							{
								const auto& s = document["s"];

								if (s.IsInt())
								{
									sequence_ = s.GetInt();
								}
							}
						}

						message_cb_(document);
					}
				}
				else
				{
					fmt::print("[xcord] Warning: zlib inflation failed on websocket message.");
				}
			}); 
		});
	}

	Websocket::~Websocket()
	{
		cleanup_();
	}

	void Websocket::connect()
	{
		websocketpp::lib::error_code e;

		const auto gateway = Discord::get().gateway();
		if (gateway.empty())
		{
			fmt::print("[xcord] Fatal: gateway url was empty.");
			return;
		}

		const auto con = client_.get_connection(fmt::format("{}&encoding=json", gateway), e);

		if (e)
		{
			fmt::print("[xcord] Fatal: failed to get connection. {}", e.message());
			return;
		}

		client_.connect(con);
		client_.run();
	}

	void Websocket::close()
	{
		cleanup_();
	}

	void Websocket::send_op(const int op)
	{
		rapidjson::Value data;
		data.SetNull();

		send_op(op, data);
	}

	void Websocket::send_op(const int op, rapidjson::Value& data, const std::string_view event_name /* = "" */)
	{
		rapidjson::Document document;
		document.SetObject();

		auto& allocator = document.GetAllocator();

		document.AddMember("op", rapidjson::Value(op), allocator);

		const auto s = sequence();
		rapidjson::Value s_value;

		if (s != -1)
		{
			s_value.SetInt(s);
		}

		document.AddMember("s", s_value, allocator);
		document.AddMember("d", data, allocator);

		if (!event_name.empty())
		{
			document.AddMember("t", rapidjson::Value(event_name.data(), event_name.size()), allocator);
		}

		rapidjson::StringBuffer buffer;
		rapidjson::Writer writer(buffer);

		document.Accept(writer);

		client_.send(handle_, buffer.GetString(), websocketpp::frame::opcode::text);
	}

	void Websocket::cleanup_()
	{
		if (connected_)
		{
			client_.close(handle_, websocketpp::close::status::going_away, "");
			client_.stop();

			connected_ = false;
		}
	}

	int Websocket::sequence() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return sequence_;
	}
}
