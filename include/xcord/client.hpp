#pragma once

#include <xcord/export.hpp>

#include <string>
#include <thread>

#include <rapidjson/document.h>

#include <xcord/websocket.hpp>

namespace xcord
{
	class EXPORT Client
	{
	public:
		Client();
		~Client();

		explicit Client(const Client&) = default; Client& operator=(const Client&) = default;
		explicit Client(Client&&) = default; Client& operator=(Client&&) = default;

		void login(const std::string& token);
		void close();
	private:
		Websocket websocket_;
		void handle_ws_message(const rapidjson::Document& document);

		std::atomic_bool heartbeat_active_ = false;
		std::thread heartbeat_;
	};
}
