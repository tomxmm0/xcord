#pragma once
#include <xcord/export.hpp>

#include <string>

namespace xcord
{
	class EXPORT Discord
	{
	public:
		static constexpr int api_version = 6;

		static Discord* get();
		Discord();

		inline std::string& api()
		{
			return api_;
		}

		inline std::string& gateway()
		{
			return gateway_;
		}
	private:
		std::string api_;
		std::string gateway_;
	};
}
