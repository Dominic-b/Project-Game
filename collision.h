#include "stdafx.h"
#pragma once

class Collision {
public:
	bool pointVsRect(int x, int y, SDL_Rect a);
	bool circleVsRect(int x, int y, int radius, SDL_Rect a);
	bool rectVsRect(SDL_Rect a, int x, int y, int w, int h);
	bool circleVsCircle(int x, int y, int radius, int x2, int y2, int radius2);
};