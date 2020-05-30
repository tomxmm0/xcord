#include <fmt/format.h>

#include <xcord/discord.hpp>
#include <xcord/util.hpp>

#include <xcord/user.hpp>

namespace xcord
{
	user::user(const rapidjson::Value& json)
	{
		id_ = json["id"].GetString();
		username_ = json["username"].GetString();
		discriminator_ = json["discriminator"].GetString();

		const auto& avatar = json["avatar"];

		if (avatar.IsNull())
		{
			avatar_url_ = fmt::format("{}{}.png", Discord::default_avatar_base, util::fast_stoull(discriminator_) % 5);
		}
		else
		{
			avatar_hash_ = avatar.GetString();
			avatar_url_ = fmt::format("{}{}/{}", Discord::avatar_base, id_, avatar_hash_);

			if (!std::strncmp(avatar_hash_.data(), "a_", 2))
			{
				avatar_url_.append(".gif");
			}
			else
			{
				avatar_url_.append(".png");
			}
		}

		timestamp_ = util::id_to_timestamp(id_);

		if (json.HasMember("bot"))
		{
			bot_ = json["bot"].GetBool();
		}
	}

	std::string_view user::id() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return id_;
	}

	std::string_view user::username() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return username_;
	}

	std::string_view user::discriminator() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return discriminator_;
	}

	std::string_view user::avatar_hash() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return avatar_hash_;
	}

	std::string_view user::avatar_url() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return avatar_url_;
	}

	std::uint64_t user::timestamp() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return timestamp_;
	}

	bool user::bot() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return bot_;
	}
}
