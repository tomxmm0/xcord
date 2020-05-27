#include <fmt/format.h>
#include <rapidjson/document.h>
#include <cpr/cpr.h>

#include "xcord/discord.hpp"

namespace xcord
{
	Discord* Discord::get()
	{
		static Discord instance;
		return &instance;
	}

	Discord::Discord() : api_(fmt::format("https://discord.com/api/v{}/", api_version))
	{
		const auto res = cpr::Get(cpr::Url(fmt::format("{}/gateway", api_)));

		rapidjson::Document document;
		document.Parse(res.text.data());

		assert(document.IsObject());
		assert(document.HasMember("url"));
		assert(document["url"].IsString());

		gateway_ = fmt::format("{}/?v={}", document["url"].GetString(), api_version);
	}
}
