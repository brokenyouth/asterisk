#pragma once

#include <string>

struct SDL_Window;
struct SDL_Renderer;

struct GameConfig
{
	std::string Title = "Asterisk";
	int Width = 800;
	int Height = 600;
	float Framerate = 60.f;
	float Fov = 90.f;
};

class Game
{
private:
	GameConfig mGameConfig;
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	bool bRunning;

private:
	bool Initialize();
	void HandleInput();
	void Render();
	void Update();

public:
	void OnKeyDown();
	void OnKeyUp();

	void Run();
	void Release();
public:
	explicit Game(const GameConfig& GConfig);
	~Game();
};