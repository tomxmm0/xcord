#pragma once

#include <xcord/export.hpp>

#include <memory>
#include <string>
#include <functional>
#include <thread>

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

namespace xcord
{
	class EXPORT Inflator
	{
	public:
		Inflator(const std::string& deflated, const std::function<void(const std::string inflated)>& callback);
		static bool is_deflated(const std::string& deflated);
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

		void cleanup_();
	};
}

