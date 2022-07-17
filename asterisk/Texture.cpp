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
    int ReqFormat = STBI_rgb_alpha;
    int Width, Height, OriginalFormat;
    mData = (uint32_t*)stbi_load(Path.c_str(), &Width, &Height, &OriginalFormat, ReqFormat);

    if (mData)
    {
#ifdef _DEBUG
        LOG_CORE_INFO("Loaded Texture... [ {} ]", Path);
#endif
    }
    else
    {
#ifdef _DEBUG
        LOG_CORE_ERROR("Failed loading Texture... [ {} ]", Path);
#endif
    }
    int Depth, Pitch;
    uint32_t PixelFormat;
    if (ReqFormat == STBI_rgb)
    {
        Depth = 24;
        Pitch = 3 * Width;
        PixelFormat = SDL_PIXELFORMAT_RGB24;
    }
    else
    {
        Depth = 32;
        Pitch = 4 * Width;
        PixelFormat = SDL_PIXELFORMAT_RGBA32;
    }

    mSurface = SDL_CreateRGBSurfaceWithFormatFrom((void*)mData, Width, Height,
        Depth, Pitch, PixelFormat);

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
    /*if (x >= 0 && y >= 0 && x < mWidth && y < mHeight)
    {

        return mData[y * mWidth + x];
    }
    return 0;*/
    return static_cast<uint32_t*>(mSurface->pixels)[y * mSurface->w + x];
}

void Texture::Release()
{
    stbi_image_free(mData);
    SDL_FreeSurface(mSurface);
}
