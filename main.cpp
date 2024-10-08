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

// undefine main at start or else sdl gets angry and tries too kill me
#undef main


int main(int, char**) {
    
    int SURFACE_X = 500;
    int SURFACE_Y = 300;

    int SURFACE_WIDTH = 400;
    int SURFACE_HEIGHT = 400;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(version_id, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);


    SDL_SetWindowResizable(window, SDL_bool(true));


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    SDL_Surface* surface_example = SDL_CreateRGBSurface(0, SURFACE_WIDTH, SURFACE_HEIGHT, 32, 0, 0, 0, 0);
    SDL_FillRect(surface_example, NULL, SDL_MapRGB(surface_example->format, 255, 255, 255));


    //SDL_FreeSurface(surface_example);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDLRenderer2_Init(renderer);
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);

    // main loop
    bool running = true;
    bool isDrawing = false;

    int lastX = 0;
    int lastY = 0;


    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                running = false;
            // Handle mouse button events
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                isDrawing = true;
                SDL_GetMouseState(&lastX, &lastY);
                // Adjust lastX and lastY to the surface area
                lastX -= SURFACE_X;
                lastY -= SURFACE_Y;
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                isDrawing = false;
            }

            // Handle mouse motion
            if (event.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                x -= SURFACE_X; // Adjust to the surface area
                y -= SURFACE_Y;

                // Draw a pixel on the surface if within bounds and if the button is pressed
                if (isDrawing) {
                    if (x >= 0 && x < SURFACE_WIDTH && y >= 0 && y < SURFACE_HEIGHT) {
                        SDL_Rect rect = { x, y, 1, 1 };
                        SDL_Rect rect_b = { lastX, lastY, 1, 1 };
                        SDL_FillRect(surface_example, &rect, SDL_MapRGB(surface_example->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f));
                        SDL_FillRect(surface_example, &rect_b, SDL_MapRGB(surface_example->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f));
                    }

                    // Optionally draw a line between the last position and the current position
                    if (lastX >= 0 && lastY >= 0 && x >= 0 && x < SURFACE_WIDTH && y >= 0 && y < SURFACE_HEIGHT) {
                        SDL_RenderDrawLine(renderer, x, y, lastX, lastY);
                        SDL_RenderDrawLine(renderer, lastX, lastY, x, y);
                    }

                    lastX = x; // Update last position
                    lastY = y; // Update last position
                }
            }
        }

        ImGui_ImplSDL2_ProcessEvent(&event);

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


        // renderer
        ImGui::Render();

        // clear the screen, render after this
        SDL_SetRenderDrawColor(renderer, 115, 140, 153, 1);
        SDL_RenderClear(renderer);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_example);
        SDL_Rect dst_rect = { SURFACE_X, SURFACE_Y, SURFACE_WIDTH, SURFACE_HEIGHT };
        SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
        SDL_DestroyTexture(texture);

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