#include "ResourceManager.h"
#include "Logger.h"

ResourceManager* ResourceManager::sInstance = nullptr;

ResourceManager::ResourceManager()
{

	LoadTexture("missing", "data/textures/dev/dev_orange.tga");

#ifdef _DEBUG
    LOG_CORE_INFO("Resource Manager initialized.");
#endif
}

void ResourceManager::ReleaseTextures() const
{
	auto Iterator = mTextures.begin();
	while (Iterator != mTextures.end())
	{
		Iterator->second->Release();
		Iterator++;
	}

#ifdef _DEBUG
    LOG_CORE_INFO("Releasing textures...");
#endif

}

void ResourceManager::Release()
{
	if (ResourceManager::sInstance)
	{
		ReleaseTextures();
		delete sInstance;
		sInstance = nullptr;

#ifdef _DEBUG
		LOG_CORE_INFO("Resource Manager released.");
#endif
	}
	else
	{
#ifdef _DEBUG
		LOG_CORE_INFO("Resource Manager already released.");
#endif
	}

}

ResourceManager* ResourceManager::Get()
{
	if (sInstance)
		return sInstance;
	sInstance = new ResourceManager();
	return sInstance;
	
}

Texture* ResourceManager::GetTexture(const std::string& Name)
{
	auto Iterator = ResourceManager::sInstance->mTextures.find(Name);
	if (Iterator != ResourceManager::sInstance->mTextures.end())
	{
		return Iterator->second.get();
	}
	return mTextures["missing"].get();
}

Texture* ResourceManager::LoadTexture(const std::string& Name, const std::string& FilePath)
{
	TexturePtr InTexture = std::make_shared<Texture>(FilePath);
	mTextures[Name] = InTexture;
	return mTextures[Name].get();
}
