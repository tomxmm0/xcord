#include <array>
#include <memory>

#include <xcord/websocket.hpp>

namespace xcord
{
	Inflator::Inflator()
	{
		if (inflateInit(&stream_) == Z_OK)
		{
			active_ = true;
		}
	}

	Inflator::~Inflator()
	{
		active_ = false;

		std::lock_guard<std::mutex> lock(mutex_);
		inflateEnd(&stream_);
	}

	std::optional<std::string> Inflator::inflate(const std::string& deflated)
	{
		std::optional<std::string> inflated;

		if (active_)
		{
			if (is_deflated(deflated))
			{
				std::string str;
				std::lock_guard<std::mutex> lock(mutex_);

				stream_.next_in = (unsigned char*)deflated.data();
				stream_.avail_in = deflated.size();

				int result = Z_OK;

				do
				{
					char buffer[4096] = { 0 };

					stream_.next_out = reinterpret_cast<unsigned char*>(buffer);
					stream_.avail_out = sizeof(buffer) - 1; 

					result = ::inflate(&stream_, Z_NO_FLUSH);

					str.append(buffer);
				} while (result == Z_OK);

				inflated = std::move(str);
			}
			else
			{
				inflated = deflated;
			}
		}

		return inflated;
	}

	bool Inflator::is_deflated(const std::string_view& deflated)
	{
		if (deflated.size() < 4)
		{
			return false;
		}

		constexpr std::array zlib_suffix{ '\xff', '\xff', '\x00', '\x00' };

		for (auto i = 0u; i < zlib_suffix.size(); ++i)
		{
			if (deflated[deflated.size() - (i + 1)] != zlib_suffix[i])
			{
				return false;
			}
		}

		return true;
	}
}
