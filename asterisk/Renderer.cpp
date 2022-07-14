/*
	Asterisk
	Redouane Chabane (July, 2022)
	Renderer.cpp - Preparing the render frame - primitives, texture, and pixel manipulation.

	render line is not mine.
*/

#include "Texture.h"
#include "Renderer.h"
#include "Logger.h"

void Renderer::SetFramePixel(int x, int y, uint32_t Color)
{
	if (x >= 0 && y >= 0 && x < mWidth && y < mHeight)
	{
		mFrameBuffer[y * mWidth + x] = Color;
	}
}

void Renderer::RenderPixel(int x, int y, uint32_t Color)
{
	SetFramePixel(x, y, Color);
}

void Renderer::RenderLine(int x1, int y1, int x2, int y2, uint32_t Color)
{
	int delta_x = abs(x2 - x1);
	int delta_y = abs(y2 - y1);
	int step_x = x1 < x2 ? 1 : -1;
	int step_y = y1 < y2 ? 1 : -1;
	int error = (delta_x > delta_y ? delta_x : -delta_y) / 2;

	// Stop drawing if pixel is off screen or we have reached the end of the line.
	while (!(x1 == x2 && y1 == y2))
	{
		SetFramePixel(x1, y1, Color);
		int e = error;
		if (e > -delta_x) error -= delta_y, x1 += step_x;
		if (e < delta_y) error += delta_x, y1 += step_y;
	}
}

void Renderer::RenderFill(uint32_t Color)
{
#pragma omp parallel for
	for (int i = 0; i < mWidth; ++i)
	{
		for (int j = 0; j < mHeight; ++j)
		{
			SetFramePixel(i, j, Color);
		}
	}
}

void Renderer::RenderRect(int x, int y, int Size, uint32_t Color)
{
#pragma omp parallel for
	for (int i = x; i < x + Size; ++i)
	{
		for (int j = y; j < y + Size; ++j)
		{
			if (i > 0 && j > 0 && i < mWidth && j < mHeight)
			{
				SetFramePixel(i, j, Color);
			}
		}
	}
}

void Renderer::RenderRect(int x, int y, int SizeX, int SizeY, uint32_t Color)
{
#pragma omp parallel for
	for (int i = x; i < x + SizeX; ++i)
	{
		for (int j = y; j < y + SizeY; ++j)
		{
			if (i > 0 && j > 0 && i < mWidth && j < mHeight)
			{
				SetFramePixel(i, j, Color);
			}
		}
	}
}

void Renderer::RenderTexture(Texture* pTexture, int x, int y)
{
	struct Vector2i
	{
		int x;
		int y;
	};
	Vector2i ScreenCoordinates = { x, y };

#pragma omp parallel for
	for (int i = 0; i < pTexture->GetWidth(); ++i)
	{
		ScreenCoordinates.y = y;

		for (int j = 0; j < pTexture->GetHeight(); ++j)
		{
			if (ScreenCoordinates.x >= 0 && ScreenCoordinates.y >= 0 &&
				ScreenCoordinates.x < mWidth && ScreenCoordinates.y < mHeight)
			{
				SetFramePixel(ScreenCoordinates.x, ScreenCoordinates.y, pTexture->GetPixelColor(i, j));
			}
			ScreenCoordinates.y++;
		}
		ScreenCoordinates.x++;
	}
}

void Renderer::RenderTextureByPart(Texture* pTexture, const glm::vec2& Coordinates, const glm::vec2& PartCoordinates, const glm::vec2& PartSize)
{
}

Renderer::Renderer(SDL_Renderer* pRenderer, int FOV, const uint32_t pWidth, const uint32_t pHeight) :
	mFOV(FOV), mWidth(pWidth), mHeight(pHeight), mRenderTarget(
		SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, pWidth, pHeight),
		SDL_DestroyTexture)
{
	mFrameBuffer = new uint32_t[mWidth * mHeight];
	mDepthBuffer = new uint32_t[mWidth * mHeight];
}

Renderer::~Renderer()
{
	delete mFrameBuffer;
	mFrameBuffer = nullptr;
	delete mDepthBuffer;
	mDepthBuffer = nullptr;
}
