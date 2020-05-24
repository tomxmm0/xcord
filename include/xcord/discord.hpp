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

		std::string& api();
		std::string& gateway();
	private:
		std::string api_;
		std::string gateway_;
	};
}
