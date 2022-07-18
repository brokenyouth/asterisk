#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include "Game.h"
#include "Logger.h"

int main(int argc, char** argv)
{
	Logger::Initialize();


	GameConfig GConfig;
	{
		GConfig.Width = 800;
		GConfig.Height = 600;
		GConfig.Fov = 68.f;
		GConfig.FPS = 120;
	}

	Game* game = new Game(GConfig);
	{
		game->Run();
	}

	delete game;

	return 0;
}
