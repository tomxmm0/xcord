#pragma once

#include <xcord/export.hpp>

#include <memory>
#include <string>
#include <functional>
#include <thread>

#include <zlib.h>

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

namespace xcord
{
	class EXPORT Inflator
	{
	public:
		Inflator(const std::string& deflated, const std::function<void(const std::string)>& cb);
		~Inflator();

		explicit Inflator(const Inflator&) = default; Inflator& operator=(const Inflator&) = default;
		explicit Inflator(Inflator&&) = default; Inflator& operator=(Inflator&&) = default;

		static bool is_deflated(const std::string& deflated);
	private:
		std::thread worker_;
	};

	class EXPORT Websocket
	{
	public:
		using ws_client = websocketpp::client<websocketpp::config::asio_tls_client>;
		using ws_handle = websocketpp::connection_hdl;

		using message_ptr = websocketpp::config::asio_tls_client::message_type::ptr;
		using context_ptr = std::shared_ptr<asio::ssl::context>;

		Websocket();
		~Websocket();

		explicit Websocket(const Websocket&) = default; Websocket& operator=(const Websocket&) = default;
		explicit Websocket(Websocket&&) = default; Websocket& operator=(Websocket&&) = default;

		void connect();
		void close();
	private:
		ws_client client_;
		ws_handle handle_;

		void cleanup_();
	};
}

