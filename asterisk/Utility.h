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
			return (a << 24) + (r << 16) + (g << 8) + b;
		}

		static uint32_t ApplyFogToPixel(uint32_t pixel, float amount)
		{
			uint8_t pixelcolor_a = (pixel >> 24) & 0xFF;
			uint8_t pixelcolor_r = (pixel >> 16) & 0xFF;
			uint8_t pixelcolor_g = (pixel >> 8) & 0xFF;
			uint8_t pixelcolor_b = pixel & 0xFF;

			pixelcolor_a = static_cast<uint8_t>((float)pixelcolor_a * amount);
			pixelcolor_r = static_cast<uint8_t>((float)pixelcolor_r * amount);
			pixelcolor_g = static_cast<uint8_t>((float)pixelcolor_g * amount);
			pixelcolor_b = static_cast<uint8_t>((float)pixelcolor_b * amount);

			return RGBAToUint32(pixelcolor_r, pixelcolor_g, pixelcolor_b, pixelcolor_a);
		}
	}

	static glm::vec2 Vector2Lerp(glm::vec2 x, glm::vec2 y, float t) {
		return x * (1.f - t) + y * t;
	}
}
