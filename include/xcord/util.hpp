#pragma once

namespace xcord
{
	namespace util
	{
		std::uint64_t fast_stoull(const std::string_view str);
		std::uint64_t fast_stoull(const char* str);

		std::uint64_t id_to_timestamp(const std::string_view id);

		bool compare_id(const std::string_view id1, const std::string_view id2);
	}
}
