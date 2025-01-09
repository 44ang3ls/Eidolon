#pragma once
#ifndef RENDER_HPP
#define RENDER_HPP


#include "src\gui.h"
#include "layers.h"
#include "SDL.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl2.h"
#include "imgui\imgui_impl_sdlrenderer2.h"
#include "imgui\ImGuiFileDialog.h"


// initialize everything we need for rendering, also creates a context for imgui and bind it to SDL2
void renderInit()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(version_id, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_SetWindowResizable(window, SDL_bool(true));

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    drawing_surface = SDL_CreateRGBSurfaceWithFormat(0, SURFACE_WIDTH, SURFACE_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
    //SDL_FillRect(drawing_surface, NULL, SDL_MapRGB(drawing_surface->format, 255, 255, 255));

    Layer default_layer("Layer 1", 1.0, false);
    Layer other_layer("Layer 2", 1.0, false);

    //SDL_FreeSurface(surface_example);

    layer_index = 0;

    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); 
    //(void) io;
    ImGui_ImplSDLRenderer2_Init(renderer);
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
}

void mergeLayers()
{
    SDL_Rect src_rect = { 0, 0, drawing_surface->w, drawing_surface->h };
    SDL_Rect dst_rect = { dst_x / 2, dst_y / 2, SURFACE_WIDTH * scroll_amt, SURFACE_HEIGHT * scroll_amt };


    for (int i = layers.size()-1; i >= 0; i--) {
        //std::cout << i;
        if (layers[i].hidden != true) {
            SDL_BlitSurface(layers[i].layer_data, &src_rect, drawing_surface, &dst_rect);
        }
    }
}


#endif