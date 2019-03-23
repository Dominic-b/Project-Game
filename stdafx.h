#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#undef main

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>

const int screenWidth = 1920 / 3 *2;
const int screenHeight = 1080 / 3 *2;
static SDL_Rect screen = {0, 0, screenWidth, screenHeight};

const double PI = 3.14159265359;
const double TAU = 2 * PI;

#include "noise.h"
static Noise noise;
#include "objects.h"
#include "events.h"
static Events events;
#include "collision.h"
static Collision collision;
#include "menu.h"
static Menu menu;
#include "render.h"
static Render render;
#include "gui.h"