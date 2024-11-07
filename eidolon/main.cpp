#pragma once


#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <Windows.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl2.h"
#include "imgui\imgui_impl_sdlrenderer2.h"
#include "imgui\ImGuiFileDialog.h"
#include "src/gui.h"
#include "src/globals.h"

// undefine main at start or else sdl gets angry and tries to kill me
#undef main
#include "stb_image_write.h"


int main(int, char**) {
    
    int SURFACE_X = 500;
    int SURFACE_Y = 300;

    int SURFACE_WIDTH = 400;
    int SURFACE_HEIGHT = 400;

    SDL_Init(SDL_INIT_EVERYTHING);
    
    window = SDL_CreateWindow(version_id, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_SetWindowResizable(window, SDL_bool(true));

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


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

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
                SDL_FillRect(surface_example, NULL, 0xFFFFFFFF);
            }

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

            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    scroll_amt *= 2.0;
                }
                else if (event.wheel.y < 0) {
                    scroll_amt *= 0.9;
                }

                if (scroll_amt <= 0) {
                    scroll_amt = 1.0f;
                }
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

                        SDL_FillRect(surface_example, &rect, SDL_MapRGBA(surface_example->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f, current_color[3]));
                        SDL_FillRect(surface_example, &rect_b, SDL_MapRGBA(surface_example->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f, current_color[3]));
                    }

                    // Optionally draw a line between the last position and the current position
                    if (lastX >= 0 && lastY >= 0 && x >= 0 && x < SURFACE_WIDTH && y >= 0 && y < SURFACE_HEIGHT) {
                        drawLine(surface_example, x, y, lastX, lastY);
                        drawLine(surface_example, lastX, lastY, x, y);
                    }

                    lastX = x; // Update last position
                    lastY = y; // Update last position

                    std::cout << lastX << " : " << lastY << "\n";
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



        ImGui::Begin("Save", nullptr, 0);

        if (ImGui::Button("Save Image")) {
            std::cout << "clicky!!!\n";
            IGFD::FileDialogConfig config; config.path = "C:/";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", config);

        }

        ImGui::End();

        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) { // will show a dialog
            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
                std::cout << filePathName << "\n";
                SDL_Surface* converted_surf = SDL_ConvertSurfaceFormat(surface_example, SDL_PIXELFORMAT_RGBA32, 0);

                std::cout << SDL_GetError() << "\n";


                stbi_write_png(filePathName.c_str(), converted_surf->w, converted_surf->h, 4, converted_surf->pixels, converted_surf->w * 4);

                SDL_FreeSurface(converted_surf);
            }

            // close
            ImGuiFileDialog::Instance()->Close();
        }


        // renderer
        ImGui::Render();


        if (scroll_amt <= 0) {
            scroll_amt = 0.1f;
        }

        // clear the screen, render after this
        SDL_SetRenderDrawColor(renderer, 115, 140, 153, 1);
        SDL_RenderClear(renderer);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_example);
        SDL_Rect src_rect = { SURFACE_X, SURFACE_Y, SURFACE_WIDTH, SURFACE_HEIGHT};
        SDL_Rect dst_rect = { SURFACE_X, SURFACE_Y, SURFACE_WIDTH, SURFACE_HEIGHT};

        SURFACE_X *= scroll_amt;
        SURFACE_Y *= scroll_amt;
        dst_rect.w = (int)(SURFACE_WIDTH * scroll_amt);
        dst_rect.h = (int)(SURFACE_HEIGHT * scroll_amt);

        std::cout << scroll_amt << "\n";

        SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
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