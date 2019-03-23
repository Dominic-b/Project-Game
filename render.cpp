#include "stdafx.h"

SDL_Texture* Render::loadTexture(std::string path, SDL_Renderer* renderer) {
	SDL_Texture* newTexture = nullptr;
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);
	return newTexture;
}

void Render::render(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int w, int h) {
	//Render to screen
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Render::renderEx(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int w, int h, double angle) {
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderCopyEx(renderer, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
}

void Render::renderSheetEx(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int x2, int y2, int w, int h, int w2, int h2, double angle) {
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	source.x = x2;
	source.y = y2;
	source.w = w2;
	source.h = h2;
	SDL_RenderCopyEx(renderer, texture, &source, &rect, angle, nullptr, SDL_FLIP_NONE);
}