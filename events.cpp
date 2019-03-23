#include "stdafx.h"

void Events::update(SDL_Event &e) {
	//mouse state
	SDL_GetMouseState(&mouseX, &mouseY);
	mouse1 = false;
	mouse2 = false;
	while (SDL_PollEvent(&e) != 0) {
		//quiting the program
		switch (e.type) {
		case SDL_QUIT:
			inGame = false;
			quit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			SDL_PumpEvents();
			switch (e.button.button) {
			case SDL_BUTTON_LEFT:
				mouse1 = true;
				mouse1held = true;
				break;
			case SDL_BUTTON_RIGHT:
				mouse2 = true;
				mouse2held = true;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			SDL_PumpEvents();
			switch (e.button.button) {
			case SDL_BUTTON_LEFT:
				mouse1held = false;
				break;
			case SDL_BUTTON_RIGHT:
				mouse2held = false;
				break;
			}
			break;
		}
	}
}