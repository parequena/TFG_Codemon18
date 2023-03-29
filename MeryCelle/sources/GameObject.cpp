#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "TextureManager.cpp"
#include "Game.cpp"

class GameObject
{
private:
	int x, y; // Where to place the Object.
	SDL_Texture *objTexture; // Texture of the Object.
	SDL_Rect srcRect, dstRect; // srcRect -> Image Load. dstRect -> Rect to draw on surface.

public:
	GameObject(const std::string textureSheet, const int &xPos, const int &yPos)
	{
		objTexture = TextureManager::LoadTexture(textureSheet);

		x = xPos;
		y = yPos;
	}

	~GameObject()
	{

	}

	inline void update()
	{
		// Image resolution.
		srcRect.h = 40;
		srcRect.w = 40;

		srcRect.x = 0;
		srcRect.y = 0;

		// Re Scale
		dstRect.x = x;
		dstRect.y = y;

		dstRect.w = srcRect.w * 2;
		dstRect.h = srcRect.h * 2;

	}

	inline void render()
	{
		SDL_RenderCopy(Render::renderer, objTexture, &srcRect, &dstRect);
	}
};

#endif