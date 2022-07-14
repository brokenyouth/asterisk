/*
    Asterisk
    Redouane Chabane (July, 2022)
    Texture.cpp - Texture asbtraction on top of stb_image.
*/

#include "Texture.h"
#include "Utility.h"
#include "Logger.h"

#include <stb_image.h>
#include <SDL_image.h>

Texture::Texture(const std::string& Path) :
    mPath(Path)
{
#ifdef _DEBUG
    LOG_CORE_INFO("Loading Texture... [ {} ]", Path);
#endif
    int Width, Height;
    mData = (uint32_t*)stbi_load(Path.c_str(), &Width, &Height, nullptr, 4);

    mWidth = Width;
    mHeight = Height;
}

Texture::~Texture()
{
    Release();
}

uint32_t Texture::GetWidth() const
{
    return mWidth;
}

uint32_t Texture::GetHeight() const
{
    return mHeight;
}

uint32_t Texture::GetPixelColor(int x, int y) const
{
    if (x >= 0 && y >= 0 && x < mWidth && y < mHeight)
    {

        return mData[y * mWidth + x];
    }
    return 0;
}

void Texture::Release()
{
    stbi_image_free(mData);
}
