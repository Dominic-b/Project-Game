#include "stdafx.h"
#pragma once

class Gui {
public:
	void drawHealth(SDL_Renderer* renderer, int health);
	void drawProgression(SDL_Renderer* renderer, int level, int progression);

	SDL_Texture* healthSheet;
	SDL_Texture* progressionSheet;
	SDL_Texture* levelSheet;

	Gui(SDL_Renderer* renderer);
};