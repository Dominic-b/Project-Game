#include "stdafx.h"
#pragma once

class Render {
public:
	SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int w, int h);
	void renderEx(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int w, int h, double angle);
	void renderSheetEx(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int x2, int y2, int w, int h, int w2, int h2, double angle);

	SDL_Rect source;
	SDL_Rect rect;
};