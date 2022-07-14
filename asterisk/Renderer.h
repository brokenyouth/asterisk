#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <SDL.h>

class Texture;

class Renderer
{
private:
	int mFOV;
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t* mFrameBuffer = nullptr;
	uint32_t* mDepthBuffer = nullptr;
	std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> mRenderTarget;

private:
	void SetFramePixel(int x, int y, uint32_t Color);

public:
	inline int GetFOV() const { return mFOV; }
	inline uint32_t GetFrameWidth() const { return mWidth; }
	inline uint32_t GetFrameHeight() const { return mHeight; }
	inline uint32_t* GetFrameBuffer() const { return mFrameBuffer; }
	inline SDL_Texture* GetRenderTarget() const { return mRenderTarget.get(); }
	void RenderPixel(int x, int y, uint32_t Color);
	void RenderLine(int x1, int y1, int x2, int y2, uint32_t Color);
	void RenderFill(uint32_t Color);
	void RenderRect(int x, int y, int Size, uint32_t Color);
	void RenderRect(int x, int y, int SizeX, int SizeY, uint32_t Color);
	void RenderTexture(Texture* pTexture, int x, int y);
	void RenderTextureByPart(Texture* pTexture, const glm::vec2& Coordinates, const glm::vec2& PartCoordinates, const glm::vec2& PartSize);
public:
	explicit Renderer(SDL_Renderer* pRenderer, int FOV, const uint32_t pWidth, const uint32_t pHeight);
	~Renderer();
};