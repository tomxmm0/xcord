#include <string_view>
#include <cstdint>

#include <xcord/util.hpp>

namespace xcord
{
	namespace util
	{
		std::uint64_t fast_stoull(const std::string_view str)
		{
			return fast_stoull(str.data());
		}

		std::uint64_t fast_stoull(const char* str)
		{
			std::uint64_t val = 0;

			while (*str) {
				val = val * 10 + (static_cast<unsigned long long>(*str++) - '0');
			}

			return val;
		}

		std::uint64_t id_to_timestamp(const std::string_view id)
		{
			return (fast_stoull(id) >> 22) + 1420070400000;
		}

		bool compare_id(const std::string_view id1, const std::string_view id2)
		{
			return fast_stoull(id1) == fast_stoull(id2);
		}
	}
}
