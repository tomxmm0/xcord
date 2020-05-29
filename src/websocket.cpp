#include <algorithm>
#include <future>

#include <rapidjson/document.h>
#include <fmt/printf.h>
#include <fmt/format.h>

#include <xcord/discord.hpp>
#include <xcord/websocket.hpp>

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
					fmt::print("{}\n", inflated.value());
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

	void Websocket::cleanup_()
	{
		if (connected_)
		{
			client_.close(handle_, websocketpp::close::status::going_away, "");
			client_.stop();

			connected_ = false;
		}
	}
}
