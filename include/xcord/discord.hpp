#pragma once
#include <xcord/export.hpp>

#include <string>
#include <memory>

namespace xcord
{
	class EXPORT Discord
	{
	public:
		static constexpr int api_version = 6;

		static constexpr std::string_view cdn = "https://cdn.discordapp.com/";
		static constexpr std::string_view avatar_base = "https://cdn.discordapp.com/avatars/";
		static constexpr std::string_view default_avatar_base = "https://cdn.discordapp.com/embed/avatars/";

		static Discord& get();
		Discord();

		inline std::string_view api() const
		{
			return api_;
		}

		inline std::string_view gateway() const
		{
			return gateway_;
		}
	private:
		std::string api_;
		std::string gateway_;
	};
}
