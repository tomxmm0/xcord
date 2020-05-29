#include <chrono>

#include <rapidjson/document.h>
#include <fmt/printf.h>

#include <xcord/client.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::bind;

namespace xcord
{
	Client::Client()
	{
		websocket_.on_message([this](const rapidjson::Document& document) -> void {
			handle_ws_message(document);
		});

		websocket_.connect();
	}

	Client::~Client()
	{
		close();
	}

	void Client::login(const std::string& token)
	{

	}

	void Client::close()
	{
		heartbeat_active_ = false;
		heartbeat_.detach();

		websocket_.close();
	}

	void Client::handle_ws_message(const rapidjson::Document& document)
	{
		if (document.HasMember("op"))
		{
			const auto opcode = document["op"].GetInt();

			switch (opcode)
			{
			case 10:
				const auto interval = document["d"]["heartbeat_interval"].GetInt();

				heartbeat_active_ = true;
				heartbeat_ = std::thread([this, interval]() -> void {
					while (heartbeat_active_)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(interval));
					
						if (heartbeat_active_)
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
				});
			}
		}
	}
}