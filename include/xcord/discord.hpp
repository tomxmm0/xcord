#pragma once
#include <xcord/export.hpp>

#include <string>

namespace xcord
{
	class EXPORT Discord
	{
	public:
		static constexpr int api_version = 6;

		static std::string api();
		static std::string gateway();
	private:
		static std::string api_;
		static std::string gateway_;
	};
}
