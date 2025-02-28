#pragma once
#include <SDL_surface.h>
#include <imgui\imgui_impl_sdl2.h>

// current version_id
const char* version_id = "Eidolon Prototype 0.1.0";

// currently selected color
static float current_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};

int pixel_brush_size = 1;

SDL_Window* window;

SDL_Renderer* renderer;

SDL_Surface* drawing_surface;

SDL_Surface* debug_surface;

int layer_index;
int tool_index;

double scroll_amt = 1.0f;
int dst_x = 0;
int dst_y = 0;

int SURFACE_X = 400;
int SURFACE_Y = 400;

int SURFACE_WIDTH = 400;
int SURFACE_HEIGHT = 400;

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;