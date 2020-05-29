#include <xcord/client.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::bind;

namespace xcord
{
	Client::Client()
	{
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
		websocket_.close();
	}
}