#pragma once

#include <glm/glm.hpp>
#include <SDL.h>
#include <string>
#include <memory>

#include "Color.h"

class Texture
{
private:
	uint32_t mWidth;
	uint32_t mHeight;
	std::string mPath;
	uint32_t* mData;
	SDL_Surface* mSurface;
	SDL_Texture* mTexture;

public:
	explicit Texture(const std::string& Path);
	~Texture();

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	uint32_t GetPixelColor(int x, int y) const;

	void Release();

	inline std::string GetPath() const { return mPath; }
	inline SDL_Surface* GetSDLSurface() const { return mSurface; }

};