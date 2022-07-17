#pragma once

#include <cstdint>
#include <string_view>
#include <glm/common.hpp>

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
			return a << 24 | b << 16 | g << 8 | r;
		}
	}

	static glm::vec2 Vector2Lerp(glm::vec2 x, glm::vec2 y, float t) {
		return x * (1.f - t) + y * t;
	}
}
