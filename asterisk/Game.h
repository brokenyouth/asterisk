#pragma once

#include <memory>
#include <string>

class State;
class Level;
class Renderer;
struct SDL_Window;
struct SDL_Renderer;

struct GameConfig
{
	std::string Title = "Asterisk";
	int Width = 800;
	int Height = 600;
	float FPS = 60.f;
	float Fov = 90.f;
};

class Game
{
private:
	GameConfig mGameConfig;
	SDL_Window* mSDLWindow = nullptr;
	SDL_Renderer* mSDLRenderer = nullptr;
	std::shared_ptr<Renderer> mGameRenderer {nullptr};
	std::unique_ptr<State> mCurrentState {nullptr};
	std::shared_ptr<Level> mCurrentLevel {nullptr};
	bool bRunning;

private:
	bool Initialize();
	void HandleInput();
	void Render();
	void Update(float DeltaTime);
	void Release();
	void InitializeGameResources();

public:
	inline const std::shared_ptr<Renderer>& GetRenderer() const { return mGameRenderer; }
	inline const std::shared_ptr<Level>& GetLevel() const { return mCurrentLevel; }

public:
	void Run();

public:
	explicit Game(const GameConfig& GConfig);
	~Game();
};