#pragma once
#ifndef RENDER_HPP
#define RENDER_HPP

#include "src\gui.h"
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


    drawing_surface = SDL_CreateRGBSurface(0, SURFACE_WIDTH, SURFACE_HEIGHT, 32, 0, 0, 0, 0);
    SDL_FillRect(drawing_surface, NULL, SDL_MapRGB(drawing_surface->format, 255, 255, 255));


    //SDL_FreeSurface(surface_example);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui_ImplSDLRenderer2_Init(renderer);
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
}



#endif