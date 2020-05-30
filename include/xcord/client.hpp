#pragma once

#include <xcord/export.hpp>

#include <string>
#include <thread>
#include <mutex>
#include <unordered_set>

#include <rapidjson/document.h>

#include <xcord/websocket.hpp>
#include <xcord/user.hpp>

namespace xcord
{
	class EXPORT client
	{
	public:
		client();
		~client();

		explicit client(const client&) = default; client& operator=(const client&) = default;
		explicit client(client&&) = default; client& operator=(client&&) = default;

		void login(const std::string_view token);

		void run();
		void close();

		std::string_view token() const;
	private:
		websocket websocket_;
		user user_;

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
