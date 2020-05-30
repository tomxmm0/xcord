#include <chrono>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <fmt/printf.h>

#include <xcord/user.hpp>
#include <xcord/client.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::bind;

namespace xcord
{
	client::client()
	{
		websocket_.on_message([this](const rapidjson::Document& document) -> void {
			handle_ws_message(document);
		});
	}

	client::~client()
	{
		close();
	}

	void client::login(const std::string_view token)
	{
		token_ = token;
	}
	
	void client::run()
	{
		websocket_.connect();
	}

	void client::close()
	{
		if (heartbeat_active_)
		{
			heartbeat_active_ = false;
			heartbeat_.detach();
		}

		websocket_.close();
	}

	std::string_view client::token() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return token_;
	}

	void client::handle_ws_message(const rapidjson::Document& document)
	{
		if (document.HasMember("op"))
		{
			const auto opcode = document["op"].GetInt();

			switch (opcode)
			{
			case 0:
			{
				handle_dispatch(document["d"], document["t"].GetString());
				break;
			}
			case 1:
			{
				send_heartbeat();
				break;
			}
			case 9:
			{
				fmt::format("[xcord] Fatal: Received invalid session opcode from websocket.");
				close();

				break;
			}
			case 10:
			{
				const auto interval = document["d"]["heartbeat_interval"].GetInt();

				heartbeat_active_ = true;
				heartbeat_ = std::thread([this, interval]() -> void {
					while (heartbeat_active_)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(interval));

						if (heartbeat_active_)
						{
							send_heartbeat();
						}
					}
					});

				if (token().empty())
				{
					fmt::print("[xcord] Fatal: token is empty. did you login?");
					close();
				}
				else
				{
					identify();
				}

				break;
			}
			default:
			{
				rapidjson::StringBuffer buffer;
				rapidjson::Writer writer(buffer);

				fmt::print("{}\n", buffer.GetString());

				break;
			}
			}
		}
	}

	void client::handle_dispatch(const rapidjson::Value& data, const std::string_view event_name)
	{
		if (event_name == "READY")
		{
			// shitcode
			user_.~user();
			new (&user_) user(data["user"]);
		}
	}

	void client::identify()
	{
		rapidjson::Document document;
		document.SetObject();

		auto& allocator = document.GetAllocator();

		document.AddMember("op", rapidjson::Value().SetInt(2), allocator);

		rapidjson::Value data;
		data.SetObject();

		data.AddMember("token", rapidjson::Value(token_.data(), token_.size()), allocator);

		rapidjson::Value properties;
		properties.SetObject();

		const auto os = []() -> std::string_view {
#ifdef WIN32
			return "windows";
#elif __linux__
			return "linux";
#endif
		}();

		if (os.empty())
		{
			fmt::print("[xcord] Fatal: the operating system is not supported.");
			close();

			return;
		}
		
		properties.AddMember("$os", rapidjson::Value(os.data(), os.size()), allocator);
		
		constexpr std::string_view library = "xcord";

		properties.AddMember("$browser", rapidjson::Value(library.data(), library.size()), allocator);
		properties.AddMember("$device", rapidjson::Value(library.data(), library.size()), allocator);

		data.AddMember("properties", properties, allocator);
		data.AddMember("compress", rapidjson::Value(true), allocator);

		document.AddMember("d", data, allocator);

		websocket_.send_raw(document);
	}

	void client::send_heartbeat()
	{
		const auto sequence = websocket_.sequence();
		auto value = rapidjson::Value();

		if (sequence != -1)
		{
			value.SetInt(sequence);
		}

		websocket_.send_op(1, value);
	}
}