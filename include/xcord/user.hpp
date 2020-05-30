#pragma once
#include <xcord/export.hpp>

#include <cstdint>
#include <string>
#include <mutex>

#include <rapidjson/document.h>

namespace xcord
{
	class EXPORT user
	{
	public:
		user() = default;
		~user() = default;
		explicit user(const rapidjson::Value& json);

		explicit user(const user&) = default; user& operator=(const user&) = default;
		explicit user(user&&) = default; user& operator=(user&&) = default;

		std::string_view id() const;
		std::string_view username() const;
		std::string_view discriminator() const;
		std::string_view avatar_hash() const;
		std::string_view avatar_url() const;
		std::uint64_t timestamp() const;
		bool bot() const;
	private:
		mutable std::mutex mutex_;

		std::string id_;
		std::string username_;
		std::string discriminator_;
		std::string avatar_hash_;
		std::string avatar_url_;
		std::uint64_t timestamp_ = 0ull;
		bool bot_ = false;
	};
}
