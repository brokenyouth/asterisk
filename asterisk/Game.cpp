#include "Game.h"
#include "Logger.h"
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

	mWindow = SDL_CreateWindow(mGameConfig.Title.c_str(), 250, 100, mGameConfig.Width, mGameConfig.Height, SDL_WINDOW_SHOWN);
	if (mWindow == nullptr) {
		LOG_CORE_ERROR("SDL_CreateWindow Error: {}", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr) {
		LOG_CORE_ERROR("SDL_CreateRenderer Error: {}", SDL_GetError());
		if (mWindow != nullptr) {
			SDL_DestroyWindow(mWindow);
		}
		SDL_Quit();
		return false;
	}

	bRunning = true;
}

void Game::HandleInput()
{
}

void Game::Render()
{
	// Clear screen
	SDL_SetRenderDrawColor(mRenderer, 124, 210, 12, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mRenderer);

	//... Draw world

	SDL_RenderPresent(mRenderer);
}

void Game::Update()
{
}

void Game::Run()
{
	Initialize();

	while (bRunning)
	{
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				bRunning = false;
			}
			/*else if (event.type == SDL_KEYDOWN)
			{

			}
			else if (event.type == SDL_KEYUP)
			{

			}*/
		}
		HandleInput();
		Update();
		Render();
	}

	Release();
}

void Game::Release()
{
	SDL_DestroyRenderer(mRenderer);
	mRenderer = nullptr;
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
	SDL_Quit();
}

