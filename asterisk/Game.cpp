#include "Game.h"
#include "Level.h"
#include "GameplayState.h"
#include "Renderer.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Utility.h"

#include <exception>
#include <fstream>
#include <memory>
#include <filesystem>
#include <format>
#include <SDL.h>

Game::Game(const GameConfig& GConfig) :
	mGameConfig(GConfig)
{
	bRunning = false;
}

Game::~Game()
{
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		LOG_CORE_ERROR("SDL_Init Error: {}", SDL_GetError());
		return false;
	}

	mSDLWindow = SDL_CreateWindow(mGameConfig.Title.c_str(), 250, 100, mGameConfig.Width, mGameConfig.Height, SDL_WINDOW_SHOWN);
	if (mSDLWindow == nullptr) {
		LOG_CORE_ERROR("SDL_CreateWindow Error: {}", SDL_GetError());
		return false;
	}

	mSDLRenderer = SDL_CreateRenderer(mSDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mSDLRenderer == nullptr) {
		LOG_CORE_ERROR("SDL_CreateRenderer Error: {}", SDL_GetError());
		if (mSDLWindow != nullptr) {
			SDL_DestroyWindow(mSDLWindow);
		}
		SDL_Quit();
		return false;
	}
	

	mGameRenderer = std::make_shared<Renderer>(mSDLRenderer, mGameConfig.Fov, mGameConfig.Width, mGameConfig.Height);

	SDL_RenderSetLogicalSize(mSDLRenderer, mGameConfig.Width, mGameConfig.Height);
	SDL_RenderSetIntegerScale(mSDLRenderer, SDL_bool(true));
	SDL_SetRenderDrawBlendMode(mSDLRenderer, SDL_BLENDMODE_BLEND);
	SDL_ShowWindow(mSDLWindow);
	//SDL_ShowCursor(SDL_DISABLE);
	SDL_SetWindowGrab(mSDLWindow, SDL_TRUE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_SetWindowFullscreen(mSDLWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);

	ResourceManager::Get(); // ?
	InputManager::Get(); // ?

	InitializeGameResources();

	bRunning = true;

	return true;
}

void Game::HandleInput()
{
}

void Game::Render()
{
	// Clear screen
	SDL_SetRenderDrawColor(mSDLRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mSDLRenderer);

	//... Render game
	mCurrentState->Render();

	//Render the texture
	SDL_UpdateTexture(mGameRenderer->GetRenderTarget(), nullptr, mGameRenderer->GetFrameBuffer(), mGameRenderer->GetFrameWidth() * sizeof(*mGameRenderer->GetFrameBuffer()));
	SDL_RenderCopy(mSDLRenderer, mGameRenderer->GetRenderTarget(), nullptr, nullptr);
	SDL_RenderPresent(mSDLRenderer);
}

void Game::Update(float DeltaTime)
{
	mCurrentState->Update(DeltaTime);
}

void Game::Run()
{
	if (Initialize())
	{
		float UpdateInterval = 1000 / mGameConfig.FPS;
		uint32_t LastUpdate = 0;
		uint32_t CurrentUpdate = 0;
		while (bRunning)
		{
			LastUpdate = SDL_GetTicks();
			SDL_Event event;
			InputManager::Get()->BeginNewFrame();
			if (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					bRunning = false;
				}
				else if (event.type == SDL_KEYDOWN)
				{
					if (event.key.repeat == 0)
						InputManager::Get()->OnKeyDown(event);
				}
				else if (event.type == SDL_KEYUP)
				{
					InputManager::Get()->OnKeyUp(event);
				}
				else if (event.type == SDL_MOUSEMOTION)
				{
					InputManager::Get()->OnMouseMotion(event);
				}
			}
			CurrentUpdate = SDL_GetTicks();
			uint32_t FrameTime = CurrentUpdate - LastUpdate;

			if (UpdateInterval > FrameTime)
			{
				//SDL_Delay(UpdateInterval - FrameTime);
			}

			HandleInput();
			Update((float) 1 / mGameConfig.FPS);
			Render();
		}

		Release();
	}
}

void Game::Release()
{
	SDL_DestroyRenderer(mSDLRenderer);
	mSDLRenderer = nullptr;
	SDL_DestroyWindow(mSDLWindow);
	mSDLWindow = nullptr;
	SDL_Quit();
}

void Game::InitializeGameResources()
{

	/****************************************/
	std::vector<std::string> TextureMappers{};

	/*
	* PART 1: Retrieve all files ending with .mapper extension
	* These are texture names mapped to a texture file
	*/

	// TODO: Hardcoded, ugly
	const std::string Path = "data";
	const std::string Extension = ".mapper";
	for (const auto& Entry : std::filesystem::directory_iterator(Path))
	{
		const std::string PathString = Entry.path().string();
		if (Util::StringEndsWith(PathString, Extension))
		{
			TextureMappers.push_back(PathString);
#ifdef _DEBUG
			LOG_CORE_INFO("Texture mapper file: {}", PathString);
#endif
		}
	}
	/*
	* PART 2: Load all textures present in the texture mappers
	*/

	// TODO: 
	for (uint32_t i = 0; i < TextureMappers.size(); ++i)
	{
		std::ifstream FileStream (TextureMappers[i].c_str());
		std::string Line;

		while (std::getline(FileStream, Line))
		{
			std::istringstream Iss(Line);
			std::string TextureName, TextureFilePath;

			if (!(Iss >> TextureName >> TextureFilePath))
			{
				const std::string Temp = std::format("Error occured while trying to load texture: {}\n", TextureName);
				throw std::runtime_error(Temp);
			}
			ResourceManager::Get()->LoadTexture(TextureName, TextureFilePath);
		}

	}

	mCurrentState = StateFactory<GameplayState>::Make(*this);
}

