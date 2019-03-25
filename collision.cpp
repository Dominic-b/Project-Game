#include "stdafx.h"

bool Collision::pointVsRect(int x, int y, SDL_Rect a) {
	if (x >= a.x && x <= a.x + a.w && y >= a.y && y <= a.y + a.h) return true;
	return false;
}

bool Collision::circleVsRect(int x, int y, int radius, SDL_Rect a) {
	float closeX = (float)x;
	float closeY = (float)y;

	if (closeX > a.x) {
		closeX = (float)a.x;
	} else if (closeX < a.x + a.w) {
		closeX = (float)a.x + a.w;
	} if (closeY > a.y) {
		closeY = (float)a.y;
	} else if (closeY < a.y + a.h) {
		closeY = (float)a.y + a.h;
	}

	float distX = x - closeX;
	float distY = y - closeY;

	return pow(distX, 2) + pow(distY, 2) < pow(radius, 2);
}

bool Collision::rectVsRect(SDL_Rect a, int x, int y, int w, int h) {
	if (a.x + a.w > x - w / 2 && a.x < x + w / 2 && a.y + a.h > y - h / 2 && a.y < y + h / 2) return true;
	return false;
}

bool Collision::circleVsCircle(int x, int y, int radius, int x2, int y2, int radius2) {
	return pow(x - x2, 2) + pow(y - y2, 2) < pow(radius + radius2, 2);
}