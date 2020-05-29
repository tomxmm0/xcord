#pragma once
#include <xcord/export.hpp>

#include <string>

namespace xcord
{
	class EXPORT Discord
	{
	public:
		static constexpr int api_version = 6;

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
