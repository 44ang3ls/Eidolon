#pragma once


#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX


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
#include "src/render.hpp"

// undefine main at start or else sdl gets angry and tries to kill me
#undef main
#include "stb_image_write.h"


int main(int, char**) {
    

    renderInit();

    // main loop
    bool running = true;
    bool isDrawing = false;

    int lastX = 0;
    int lastY = 0;

    SDL_Surface* debug_surface = SDL_CreateRGBSurface(0, SURFACE_WIDTH, SURFACE_HEIGHT, 32, 0, 0, 0, 0);

    int dst_x = 0;
    int dst_y = 0;


    while (running) {
        SDL_Event event;


        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
                SDL_FillRect(drawing_surface, NULL, 0xFFFFFFFF);
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                dst_y -= 5;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                dst_y += 5;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {
                dst_x -= 5;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
                dst_x += 5;
            }

            // Handle mouse button events
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                isDrawing = true;
                int r_x, r_y;
                SDL_GetMouseState(&r_x, &r_y);
                std::cout << "RAWMOUSESTATE : " << r_x << " : " << r_y << "\n";
                SDL_GetMouseState(&lastX, &lastY);
                // Adjust lastX and lastY to the surface area
                lastX -= SURFACE_X;
                lastY -= SURFACE_Y;

                lastX /= scroll_amt;
                lastY /= scroll_amt;

            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                isDrawing = false;
            }

            if (event.type == SDL_MOUSEWHEEL) {
                int m_pos_x, m_pos_y; 
                SDL_GetMouseState(&m_pos_x, &m_pos_y);

                int square_x = (SCREEN_WIDTH - SURFACE_WIDTH) / 2;
                int square_y = (SCREEN_HEIGHT - SURFACE_HEIGHT) / 2;

                int rel_x, rel_y;
                rel_x = m_pos_x - square_x;
                rel_y = m_pos_y - square_y;


                if (event.wheel.y > 0) {
                    scroll_amt *= 2.0;
                }
                else if (event.wheel.y < 0) {
                    scroll_amt *= 0.9;
                }

                if (scroll_amt <= 0) {
                    scroll_amt = 1.0f;
                }

                dst_x = m_pos_x - (rel_x * scroll_amt);
                dst_y = m_pos_y - (rel_y * scroll_amt);
            }

            // Handle mouse motion
            if (event.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                x -= SURFACE_X; // Adjust to the surface area
                y -= SURFACE_Y;

                x /= scroll_amt;
                y /= scroll_amt;

                // Draw a pixel on the surface if within bounds and if the button is pressed
                if (isDrawing) {
                    if (x >= 0 && x < SURFACE_WIDTH && y >= 0 && y < SURFACE_HEIGHT) {
                        SDL_Rect rect = { x, y, 1, 1 };
                        SDL_Rect rect_b = { lastX, lastY, 1, 1 };

                        SDL_FillRect(drawing_surface, &rect, SDL_MapRGBA(drawing_surface->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f, current_color[3]));
                        SDL_FillRect(drawing_surface, &rect_b, SDL_MapRGBA(drawing_surface->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f, current_color[3]));
                    }

                    // Optionally draw a line between the last position and the current position
                    if (lastX >= 0 && lastY >= 0 && x >= 0 && x < SURFACE_WIDTH && y >= 0 && y < SURFACE_HEIGHT) {
                        drawLine(drawing_surface, x, y, lastX, lastY);
                        drawLine(drawing_surface, lastX, lastY, x, y);
                    }

                    lastX = x; // Update last position
                    lastY = y; // Update last position

                    std::cout << lastX << " : " << lastY << "\n";
                }
            }
        }

        ImGui_ImplSDL2_ProcessEvent(&event);
        
        createGuiElements();

        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) { // will show a dialog
            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
                std::cout << filePathName << "\n";
                SDL_Surface* converted_surf = SDL_ConvertSurfaceFormat(drawing_surface, SDL_PIXELFORMAT_RGBA32, 0);

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

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, drawing_surface);
        SDL_Rect src_rect = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT};
        SDL_Rect dst_rect = { dst_x / 2, dst_y / 2, SURFACE_WIDTH * scroll_amt, SURFACE_HEIGHT * scroll_amt };
        
        //dst_rect.x = fmax(0, fmin(dst_rect.x, SCREEN_WIDTH - dst_rect.w)) * scroll_amt;
        //dst_rect.y = fmax(0, fmin(dst_rect.y, SCREEN_HEIGHT - dst_rect.y)) * scroll_amt;

        SURFACE_X = dst_x / 2;
        SURFACE_Y = dst_y / 2;

        SDL_Rect debug_src_rect = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };
        SDL_Rect debug_dst_rect = { SURFACE_X, SURFACE_Y , SURFACE_WIDTH, SURFACE_HEIGHT };
        

        SDL_FillRect(debug_surface, &debug_dst_rect, 1000);

        SDL_Texture* debug_texture = SDL_CreateTextureFromSurface(renderer, debug_surface);

        SDL_RenderCopy(renderer, debug_texture, &debug_src_rect, &debug_dst_rect);

        //std::cout << scroll_amt << "\n";

        SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
        SDL_DestroyTexture(texture);
        SDL_DestroyTexture(debug_texture);

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