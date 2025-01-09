#pragma once


#define STB_IMAGE_IMPLEMENTATION
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
#include "src/layers.h"
#include "src/render.hpp"

// undefine main at start or else sdl gets angry and tries to kill me
#undef main
#include "stb_image_write.h"
#include "stb_image.h"


int main(int, char**) {
    

    renderInit();

    // main loop
    bool running = true;
    bool is_drawing = false;

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

            // might remove later, makes the entire drawing surface white
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB) {
                SDL_FillRect(drawing_surface, NULL, 0xFFFFFFFF);
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                dst_y -= 5 * scroll_amt;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                dst_y += 5 * scroll_amt;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {
                dst_x -= 5 * scroll_amt;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
                dst_x += 5 * scroll_amt;
            }

            // Handle mouse button events
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                is_drawing = true;
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
                is_drawing = false;
            }

            // scrolling logic
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

            // handle mouse motion
            if (event.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                x -= SURFACE_X; // adjust to the surface area
                y -= SURFACE_Y;

                x /= scroll_amt;
                y /= scroll_amt;

                // draw a pixel on the surface if it is within bounds and if the button is pressed, use line drawing function to draw lines to allow for smoother drawing
                if (is_drawing ) {
                    if (x >= 0 && x < SURFACE_WIDTH && y >= 0 && y < SURFACE_HEIGHT) {
                        SDL_Rect rect = { x, y, 1, 1 };
                        SDL_Rect rect_b = { lastX, lastY, 1, 1 };


                        SDL_FillRect(layers[layer_index].layer_data, &rect, SDL_MapRGBA(layers[layer_index].layer_data->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f, current_color[3]));
                        SDL_FillRect(layers[layer_index].layer_data, &rect_b, SDL_MapRGBA(layers[layer_index].layer_data->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f, current_color[3]));
                    }

                    // optionally draw a line between the last position and the current position
                    if (lastX >= 0 && lastY >= 0 && x >= 0 && x < SURFACE_WIDTH && y >= 0 && y < SURFACE_HEIGHT) {

                        drawLine(layers[layer_index].layer_data, x, y, lastX, lastY);
                        drawLine(layers[layer_index].layer_data, lastX, lastY, x, y);
                    }

                    lastX = x; // update last position
                    lastY = y; // update last position

                    std::cout << lastX << " : " << lastY << "\n";
                }
            }
        }

        ImGui_ImplSDL2_ProcessEvent(&event);
        
        createGuiElements();

        // file dialog for saving
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) { // will show a dialog
            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
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

        if (ImGuiFileDialog::Instance()->Display("ChooseImportDlgKey")) { // will show a dialog
            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

                std::cout << filePathName << "\n";

                int width, height = 0;
                int channels = 0;
                int rec_channel = STBI_rgb_alpha;


                unsigned char* dat = stbi_load(filePathName.c_str(), &width, &height, &channels, rec_channel);

                if (dat == NULL) {
                    SDL_Log("Loading image failed: %s", stbi_failure_reason());
                    ImGuiFileDialog::Instance()->Close();
                }


                Uint32 r_mask, g_mask, b_mask, a_mask;

                #if SDL_BYTEORDER == SDL_BIG_ENDIAN // beeg endian
                    int shift = (req_format == STBI_rgb) ? 8 : 0;
                    r_mask = 0xff000000 >> shift;
                    g_mask = 0x00ff0000 >> shift;
                    b_mask = 0x0000ff00 >> shift;
                    a_mask = 0x000000ff >> shift;
                #else // little endian
                    r_mask = 0x000000FF;
                    g_mask = 0x0000FF00;
                    b_mask = 0x00FF0000;
                    a_mask = (rec_channel == STBI_rgb) ? 0 : 0xFF000000;
                #endif


                int depth, pitch;

                if (rec_channel == STBI_rgb) {
                    depth = 24;
                    pitch = 3 * width;
                }
                else {
                    depth = 32;
                    pitch = 4 * width;
                }


                if (dat)
                {
                    SURFACE_HEIGHT = height;
                    SURFACE_WIDTH = width;

                    drawing_surface = SDL_CreateRGBSurfaceFrom((void*) dat, width, height, depth, pitch, r_mask, g_mask, 
                        b_mask, a_mask);

                    std::cout << channels << "\n";
                    std::cout << "Potential import errors: " << SDL_GetError() << "\n";
                    std::cout << drawing_surface->w << " : " << drawing_surface->h << "\n";
                }
                else
                {
                    std::cout << stbi_failure_reason() << "\n";
                }

                // eventually free data eventually
                //stbi_image_free(dat);
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

        SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, 0, 0, 0);

        mergeLayers();

        if (drawing_surface)
        {
            //std::cout << "potential texture errors : " << SDL_GetError() << "\n";
            texture = SDL_CreateTextureFromSurface(renderer, drawing_surface);
        }

        SDL_Rect src_rect = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };
        SDL_Rect dst_rect = { dst_x / 2, dst_y / 2, SURFACE_WIDTH * scroll_amt, SURFACE_HEIGHT * scroll_amt };
        

        SURFACE_X = dst_x / 2;
        SURFACE_Y = dst_y / 2;


        SDL_Rect debug_src_rect = { 0, 0, SURFACE_WIDTH, SURFACE_HEIGHT };
        SDL_Rect debug_dst_rect = { SURFACE_X, SURFACE_Y, SURFACE_WIDTH, SURFACE_HEIGHT };
        

        //SDL_FillRect(debug_surface, &debug_dst_rect, 89900);

        SDL_Texture* debug_texture = SDL_CreateTextureFromSurface(renderer, debug_surface);

        //SDL_RenderCopy(renderer, debug_texture, &debug_src_rect, &debug_dst_rect);

        //std::cout << scroll_amt << "\n";

        SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);

        // destroy textures at the end of the frame as to not create any potential memory leaks
        SDL_DestroyTexture(texture);
        //SDL_DestroyTexture(debug_texture);

        // render ImGui
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