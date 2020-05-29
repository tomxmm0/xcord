#pragma once

#include <xcord/export.hpp>

#include <memory>
#include <string>
#include <functional>
#include <optional>
#include <mutex>

#include <zlib.h>

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

namespace xcord
{
	class EXPORT Inflator
	{
	public:
		Inflator();
		~Inflator();

		explicit Inflator(const Inflator&) = default; Inflator& operator=(const Inflator&) = default;
		explicit Inflator(Inflator&&) = default; Inflator& operator=(Inflator&&) = default;

		std::optional<std::string> inflate(const std::string& deflated);
		static bool is_deflated(const std::string_view& deflated);
	private:
		z_stream stream_ = { 0 };

		std::mutex mutex_;
		std::atomic_bool active_ = false;
	};

	class EXPORT Websocket
	{
	public:
		using ws_client = websocketpp::client<websocketpp::config::asio_tls_client>;
		using ws_handle = websocketpp::connection_hdl;

		using message_ptr = websocketpp::config::asio_tls_client::message_type::ptr;

		Websocket();
		~Websocket();

		explicit Websocket(const Websocket&) = default; Websocket& operator=(const Websocket&) = default;
		explicit Websocket(Websocket&&) = default; Websocket& operator=(Websocket&&) = default;

		void connect();
		void close();
	private:
		ws_client client_;
		ws_handle handle_;

		Inflator inflator_;

		void cleanup_();
		bool connected_ = false;
	};
}

