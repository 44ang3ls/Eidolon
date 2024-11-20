#pragma once
#ifndef RENDER_HPP
#define RENDER_HPP

#include "src\gui.h"
#include "SDL.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl2.h"
#include "imgui\imgui_impl_sdlrenderer2.h"
#include "imgui\ImGuiFileDialog.h"

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
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDLRenderer2_Init(renderer);
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
}

void renderGui()
{
    // start the imgui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once); // Fixed position
    ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_Once); // Fixed size
    ImGui::Begin("Fixed Window", nullptr, 0);

    // windows content
    ImGui::Text("Fixed pos");
    ImGui::Button("A Button", ImVec2(100, 30));
    static char buffer[128] = "input text";
    ImGui::InputText("inp", buffer, IM_ARRAYSIZE(buffer), 0);

    ImGui::ColorEdit4("color wheel", current_color);

    ImGui::End();



    ImGui::Begin("Save", nullptr, 0);

    if (ImGui::Button("Save Image")) {
        std::cout << "clicky!!!\n";
        IGFD::FileDialogConfig config; config.path = "C:/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", config);

    }

    ImGui::End();


    ImGui::Begin("New", nullptr, 0);

    static char width_buffer[128] = "400";
    ImGui::InputText("Width", width_buffer, IM_ARRAYSIZE(width_buffer), 0);
    static char height_buffer[128] = "500";
    ImGui::InputText("Height", height_buffer, IM_ARRAYSIZE(height_buffer), 0);

    if (ImGui::Button("New Image")) {
        SDL_FreeSurface(drawing_surface);
        SURFACE_WIDTH = atoi(width_buffer);
        SURFACE_HEIGHT = atoi(height_buffer);
        drawing_surface = SDL_CreateRGBSurface(0, SURFACE_WIDTH, SURFACE_HEIGHT, 32, 0, 0, 0, 0);
        SDL_FillRect(drawing_surface, NULL, 0xFFFFFFFF);


        std::cout << SURFACE_WIDTH << "\n";
        std::cout << SURFACE_HEIGHT << "\n";
    }

    ImGui::End();
}

void renderProcess()
{
	
}

#endif