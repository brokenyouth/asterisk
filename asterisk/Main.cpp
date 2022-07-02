#include "Game.h"
#include "Logger.h"

int main(int argc, char** argv)
{
	Logger::Initialize();


	GameConfig GConfig;
	GConfig.Fov = 68.f;

	Game* game = new Game(GConfig);
	game->Run();

	delete game;

	return 0;
}
