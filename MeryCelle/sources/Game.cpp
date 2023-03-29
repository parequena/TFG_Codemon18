#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <stdlib.h>
#include "GameObject.cpp"
#include "Map.cpp"

#define DF_MainCharSprite "sprites/main.png"
#define DF_1 "sprites/R.bmp"

// Flags for Window creation.
const Uint32 flags = 0;

class Game
{
private:
	int cnt;
	bool isRunning = false;
	SDL_Window *window = nullptr;
	GameObject *mainPlayer = nullptr, *otherObject = nullptr;
	Map *map = nullptr;
	Render rend;

public:

	// Ctor
	Game(const std::string &title, const int &x, const int &y, const int &w, const int &h)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) == 0) // All good.
		{
			window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

			if(!window)
				std::cerr << "Error: Window creation failed.\n";

			rend.renderer = SDL_CreateRenderer(window, -1, 0);
			SDL_SetRenderDrawColor(rend.renderer, 255, 255, 255, 0); // Render to white.

			if(!rend.renderer)
				std::cerr << "Error: Renderer creation failed.\n";

			isRunning = true;
		}
		else
			std::cerr << "Error: Init failed.\n";

		mainPlayer = new GameObject(DF_MainCharSprite, 0, 0);

		std::vector<std::string> names;
		names.push_back("sprites/black.png");
		names.push_back("sprites/grey.png");
		names.push_back("sprites/white.png");

		map = new Map(names);
	}

	// Dtor
	~Game()
	{

	}

	inline void handleEvents()
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
			case SDL_QUIT:
				isRunning = false;
				break;
		}
	}

	inline void update()
	{
		mainPlayer->update();
	}

	inline void render()
	{
		SDL_RenderClear(rend.renderer);
		// Textures to rend.
		// Background -> Top.
		// map->drawMap();
		mainPlayer->render();
		SDL_RenderPresent(rend.renderer);
	}

	inline void clear()
	{
		if(window != nullptr)
		{
			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(rend.renderer);
			SDL_Quit();
		}
	}

	inline bool running()
	{
		return isRunning;
	}
};

#endif