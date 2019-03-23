#include "stdafx.h"

Gui::Gui(SDL_Renderer* renderer) {
	healthSheet = render.loadTexture("Assets/Images/healthBar.png", renderer);
	progressionSheet = render.loadTexture("Assets/Images/progressionBar.png", renderer);
	levelSheet = render.loadTexture("Assets/Images/Levels.png", renderer);
}

void Gui::drawHealth(SDL_Renderer* renderer, int health) {
	render.renderSheetEx(renderer, healthSheet, 0, 0, 0, 0, 256, 32, 128, 16, 0);
	render.renderSheetEx(renderer, healthSheet, 0, 0, 0, 16, health * 2.56, 32, 128, 16, 0);
}

void Gui::drawProgression(SDL_Renderer* renderer, int level, int progression) {
	render.renderSheetEx(renderer, progressionSheet, (screenWidth / 2) - 128, screenHeight - 32, 0, 0, 256, 32, 128, 16, 0);
	render.renderSheetEx(renderer, progressionSheet, (screenWidth / 2) - 128, screenHeight - 32, 0, 16, progression * 2.56, 32, 128, 16, 0);
	render.renderSheetEx(renderer, levelSheet, (screenWidth / 2) - 48, screenHeight - 52, 0, level * 10, 96, 20, 48, 10, 0);
}