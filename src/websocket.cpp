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
			const Websocket::context_ptr context = std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);
			websocketpp::lib::error_code ec;

			context->set_options(asio::ssl::context::default_workarounds |
				asio::ssl::context::no_sslv2 |
				asio::ssl::context::no_sslv3 |
				asio::ssl::context::single_dh_use, ec);

			assert(!ec);

			return context;
		});

		client_.set_open_handler([this](const websocketpp::connection_hdl hdl) -> auto {
			handle_ = hdl;
		});

		client_.set_message_handler([this](const websocketpp::connection_hdl, const message_ptr message) -> auto {
			// TODO
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
