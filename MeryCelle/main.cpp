#include <iostream>

#define DF_ProjectName "Merycelle"
#define DF_windowWidth 640
#define DF_windowHeight 320
#define DF_FPS 60

#include "sources/Game.cpp"
#include "sources/Render.cpp"

const int frameDelay = 1000/DF_FPS;
Game *game = nullptr;
SDL_Renderer *Render::renderer = nullptr;

int main(int argc, char *argv[])
{
	Uint32 frameStart;
	int frameTime;

	game = new Game(DF_ProjectName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DF_windowWidth, DF_windowHeight);

	while(game->running())
	{
		frameStart = SDL_GetTicks(); // Miliseconds it's been from first SDL Init.
		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;
		if(frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}

	game->clear();
	return 0;
}