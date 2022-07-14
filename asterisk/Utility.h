#pragma once

#include <cstdint>
#include <string_view>

namespace Util
{
	static bool StringEndsWith(std::string_view Str, std::string_view Suffix)
	{
		return Str.size() >= Suffix.size() && 0 == Str.compare(Str.size() - Suffix.size(), Suffix.size(), Suffix);
	}

	static bool StringStartsWith(std::string_view Str, std::string_view Prefix)
	{
		return Str.size() >= Prefix.size() && 0 == Str.compare(0, Prefix.size(), Prefix);
	}

	namespace Color
	{
		static uint32_t RGBAToUint32(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			return r << 24 | g << 16 | b << 8 | a;
		}
	}
}
