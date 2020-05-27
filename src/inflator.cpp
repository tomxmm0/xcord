#include <cassert>
#include <xcord/websocket.hpp>

namespace xcord
{
	Inflator::Inflator(const std::string& deflated, const std::function<void(const std::string)>& cb)
	{
		worker_ = std::thread([deflated, cb]() -> void {
			if (is_deflated(deflated))
			{
				z_stream stream = { 0 };
				assert(inflateInit(&stream) == Z_OK);

				std::string inflated;
				stream.next_in = (unsigned char*)deflated.data();
				stream.avail_in = deflated.size();

				int result = Z_OK;

				do
				{
					char buffer[1028] = { 0 };

					stream.next_out = reinterpret_cast<unsigned char*>(buffer);
					stream.avail_out = sizeof(buffer);

					result = ::inflate(&stream, 0);

					inflated.append(buffer);
				} while (result == Z_OK);

				/*
					Z_BUF_ERROR indicates that the given buffer was too small for the inflated chunck.
					It is not fatal and `inflate` can be called once again with more output space.
				*/
				assert(result == Z_STREAM_END || result == Z_BUF_ERROR);
				assert(inflateEnd(&stream) == Z_OK);

				cb(std::move(inflated));
			}
			else
			{
				cb(deflated);
			}
		});
	}

	Inflator::~Inflator()
	{
		worker_.join();
	}

	bool Inflator::is_deflated(const std::string& deflated)
	{
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
