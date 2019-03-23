#include "stdafx.h"
#pragma once

class Menu {
public:
	SDL_Rect playButton;
	bool playButtonHover;

	SDL_Rect exitButton;
	bool exitButtonHover;

	void drawMainMenu(SDL_Renderer* renderer);
	void updateMainMenu(int x, int y, bool click, bool &quit, bool &inGame);//mouse x and y, and mouse click

	Menu();
};