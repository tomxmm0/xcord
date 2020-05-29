#pragma once

#include <xcord/export.hpp>

#include <string>
#include <thread>
#include <mutex>

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

		void run();
		void close();

		const std::string_view token() const;
	private:
		Websocket websocket_;
		mutable std::mutex mutex_;

		void handle_ws_message(const rapidjson::Document& document);
		void handle_dispatch(const rapidjson::Value& data, const std::string_view event_name);

		void identify();
		void send_heartbeat();

		std::atomic_bool heartbeat_active_ = false;
		std::thread heartbeat_;

		std::string token_;
	};
}
