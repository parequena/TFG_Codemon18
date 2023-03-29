#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "SDL.h"
#include "SDL_image.h"
#include "Render.cpp"

class TextureManager
{
public:
	static SDL_Texture *LoadTexture(const std::string fileName)
	{
		SDL_Surface *tmpSurface = IMG_Load(fileName.c_str());
		SDL_Texture *tex = SDL_CreateTextureFromSurface(Render::renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);

		return tex;
	}

    inline static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst)
    {
        SDL_RenderCopy(Render::renderer, tex, &src, &dst);
    }

};

#endif