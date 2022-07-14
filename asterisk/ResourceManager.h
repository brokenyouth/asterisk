#pragma once

#include <memory>
#include <unordered_map>

#include "Texture.h"

typedef std::shared_ptr<Texture> TexturePtr;

class ResourceManager
{
private:
	static ResourceManager* sInstance;
	std::unordered_map<std::string, TexturePtr> mTextures;

private:
	ResourceManager();
	virtual ~ResourceManager() {}

	void ReleaseTextures() const;

public:
	void Release();

	static ResourceManager* Get();
	Texture* GetTexture(const std::string& Name);

	Texture* LoadTexture(const std::string& Name, const std::string& FilePath);
};