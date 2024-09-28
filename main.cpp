#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <Windows.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl2.h"
#include "imgui\imgui_impl_sdlrenderer2.h"
#include "src/gui.h"
#include "src/globals.h"

// undefine main or else sdl gets angry
#undef main


int main(int, char**) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(version_id, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);


    SDL_SetWindowResizable(window, SDL_bool(true));


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    SDL_Surface* surface_example = IMG_Load("textures/q1.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_example);

    SDL_FreeSurface(surface_example);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDLRenderer2_Init(renderer);
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);

    // main loop
    bool running = true;

    double angle = 0.0f;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        angle += 1.0f;
        angle = fmod(angle, 360.0);

        // start the imgui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // gui code goes here
        ImGui::Begin("hiiii");
        ImGui::Text("hi");
        ImGui::End();

        ImGui::Begin("2");
        ImGui::Text("177013");
        ImGui::End();

        ImGui::Begin("iiiiiiiiiiiiii");
        ImGui::Text("what the what the");
        ImGui::End();


        int DATA_SIZE = 2;

        float data[2] = { 0.0f };

        for (int i = 0; i < DATA_SIZE - 1; i++) {
            data[i] = data[i + 1];
        }

        data[DATA_SIZE - 1] = angle;

        ImGui::Begin("CURRENT STABILITY");
        ImGui::PlotLines("CURRENT STABILITY", data, 2);
        ImGui::End();


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


        createSomething();


        // renderer
        ImGui::Render();

        // clear the screen, render after this
        SDL_SetRenderDrawColor(renderer, 115, 140, 153, 1);
        SDL_RenderClear(renderer);



        SDL_Rect dstRect = { 600, 310, 200, 200 }; // Position and size
        SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);

        // Render ImGui
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    // cleanup after everything is done
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}