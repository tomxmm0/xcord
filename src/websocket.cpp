#include <algorithm>

#include <rapidjson/document.h>
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
			handle_ = hdl;
		});

		client_.set_message_handler([this](const websocketpp::connection_hdl, const message_ptr message) -> auto {
			Inflator(message->get_payload(), [](const std::string message) -> void {
				fmt::print("{}\n", message);
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
		const auto con = client_.get_connection(fmt::format("{}&encoding=json&compress=zlib-stream", Discord::get()->gateway()), e);

		assert(!e);
		
		client_.connect(con);
		client_.run();
	}

	void Websocket::close()
	{
		cleanup_();
	}

	void Websocket::cleanup_()
	{
		client_.close(handle_, websocketpp::close::status::going_away, "");
	}
}
