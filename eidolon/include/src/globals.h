#pragma once
#include <imgui\imgui_impl_sdl2.h>
#ifndef GLOBALS_H
#define GLOBALS_H

// current version_id
const char* version_id = "Eidolon Prototype 0.0.1b";

// currently selected color
static float current_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

int pixel_brush_size = 1;

SDL_Window* window;

SDL_Renderer* renderer;

SDL_Surface* drawing_surface;

int scroll_amt = 1.0f;

int SURFACE_X = 400;
int SURFACE_Y = 400;

int SURFACE_WIDTH = 400;
int SURFACE_HEIGHT = 400;


#endif