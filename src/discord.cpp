#include <fmt/format.h>
#include <rapidjson/document.h>
#include <cpr/cpr.h>

#include "xcord/discord.hpp"

namespace xcord
{
	std::string Discord::api_ = fmt::format("https://discord.com/api/v{}/", Discord::api_version);
	std::string Discord::gateway_;

	std::string Discord::api()
	{
		return api_;
	}

	std::string Discord::gateway()
	{
		if (gateway_.empty())
		{
			auto res = cpr::Get(cpr::Url(fmt::format("{}/gateway", api_)));

			rapidjson::Document document;
			document.Parse(res.text.data());

			assert(document.HasMember("url"));
			assert(document["url"].IsString());
	
			gateway_ = document["url"].GetString();
		}

		return gateway_;
	}
}
