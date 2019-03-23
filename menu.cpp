#include "stdafx.h"

Menu::Menu() {
	playButton.w = 100;
	playButton.h = 25;
	playButton.x = screenWidth / 2 - playButton.w / 2;
	playButton.y = 100;
	exitButton.w = 100;
	exitButton.h = 25;
	exitButton.x = playButton.x;
	exitButton.y = 200;
}

void Menu::drawMainMenu(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
	if (playButtonHover) {
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
	}

	SDL_RenderFillRect(renderer, &playButton);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);

	if (exitButtonHover) {
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
	}

	SDL_RenderFillRect(renderer, &exitButton);
}

void Menu::updateMainMenu(int x, int y, bool click, bool &quit, bool &inGame) {
	playButtonHover = false;
	exitButtonHover = false;

	if (collision.pointVsRect(x, y, playButton)) playButtonHover = true;
	else if (collision.pointVsRect(x, y, exitButton)) exitButtonHover = true;

	if (click) {
		if (playButtonHover) inGame = true;
		else if (exitButtonHover) quit = true;
	}
}