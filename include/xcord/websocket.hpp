#pragma once

#include <xcord/export.hpp>

#include <string>
#include <functional>
#include <optional>
#include <mutex>

#include <zlib.h>
#include <rapidjson/document.h>

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

namespace xcord
{
	class EXPORT inflator
	{
	public:
		inflator();
		~inflator();

		explicit inflator(const inflator&) = default; inflator& operator=(const inflator&) = default;
		explicit inflator(inflator&&) = default; inflator& operator=(inflator&&) = default;

		std::optional<std::string> inflate(const std::string_view deflated);
		static bool is_deflated(const std::string_view deflated);
	private:
		z_stream stream_ = { 0 };

		std::mutex mutex_;
		std::atomic_bool active_ = false;
	};

	class EXPORT websocket
	{
	public:
		using ws_client = websocketpp::client<websocketpp::config::asio_tls_client>;
		using ws_handle = websocketpp::connection_hdl;

		using message_ptr = websocketpp::config::asio_tls_client::message_type::ptr;
		using message_cb = std::function<void(const rapidjson::Document&)>;

		websocket();
		~websocket();

		explicit websocket(const websocket&) = default; websocket& operator=(const websocket&) = default;
		explicit websocket(websocket&&) = default; websocket& operator=(websocket&&) = default;

		void connect();
		void close();

		void send_op(const int op);
		void send_op(const int op, rapidjson::Value& data, const std::string_view event_name = "");

		void send_raw(const rapidjson::Document& document);

		inline void on_message(const message_cb& callback)
		{
			message_cb_ = callback;
		}

		int sequence() const;
	private:
		ws_client client_;
		ws_handle handle_;

		inflator inflator_;

		message_cb message_cb_;

		mutable std::mutex mutex_;

		void cleanup_();
		bool connected_ = false;

		int sequence_ = -1;
	};
}

