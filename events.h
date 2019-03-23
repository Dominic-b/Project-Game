#include "stdafx.h"
#pragma once

class Events {
public:
	void update(SDL_Event &e);

	int mouseX, mouseY;
	bool mouse1, mouse2;
	bool mouse1held, mouse2held;

	const Uint8 *keyboard_state_array;

	bool quit, inGame;
};