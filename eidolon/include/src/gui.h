#pragma once

#include <SDL_surface.h>
#include <stdio.h>

#include <functional>
#include <iostream>
#include <vector>

#include "globals.h"
#include "imgui\imgui.h"
#include "layers.h"
#include "fa.h"
#include "IconsFontAwesome6.h"

bool show_new_prompt = false;
bool show_import_prompt = false;

// creates a basic ImGui element, can add a vector of functions to execute in order, call std::bind when giving the functions argument, supply function args in
void createBasicGuiElement(bool set_size, ImVec2 size = {100, 100}, ImVec2 position = {0, 0}, ImGuiCond_ size_flags = ImGuiCond_None, ImGuiCond_ pos_flags = ImGuiCond_None, const char* title = "", ImGuiWindowFlags flags = 0, std::vector<std::function<void()>> functions = {}, bool* open = 0, float alpha = 1.0f) {
    if (set_size) {
        ImGui::SetNextWindowSize(size, size_flags);
        ImGui::SetNextWindowPos(position, pos_flags);
        ImGui::SetNextWindowBgAlpha(alpha);
    }

    ImGui::Begin(title, nullptr, flags);

    // load all the functions given
    if (functions.size() != 0) {
        for (int i = 0; i < functions.size(); i++) {
            functions[i]();
        }
    }

    ImGui::End();
}

// draw a line, used for smoother lines
void drawLine(SDL_Surface* surface, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        SDL_Rect rect = {x0, y0, 1, 1};
        SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f, current_color[3] * 255.0f));

        // check for the end condition
        if (x0 == x1 && y0 == y1) break;

        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void newImagePrompt()
{
    if (show_new_prompt) {
        ImGui::OpenPopup("New Image");

        if (ImGui::BeginPopupModal("New Image", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            // input buffers
            static char width_buffer[128] = "400";
            ImGui::InputText("Width", width_buffer, IM_ARRAYSIZE(width_buffer), 0);
            static char height_buffer[128] = "500";
            ImGui::InputText("Height", height_buffer, IM_ARRAYSIZE(height_buffer), 0);

            if (ImGui::Button("Sizes")) {
                SDL_FreeSurface(drawing_surface);
                SURFACE_WIDTH = atoi(width_buffer);
                SURFACE_HEIGHT = atoi(height_buffer);

                clearLayers();
                // drawing_surface = SDL_CreateRGBSurfaceWithFormat(0, SURFACE_WIDTH, SURFACE_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
                SDL_FillRect(drawing_surface, NULL, SDL_MapRGBA(drawing_surface->format, 0, 0, 0, 0));

                Layer("Layer 1", 1.0, false);

                std::cout << SURFACE_WIDTH << "\n";
                std::cout << SURFACE_HEIGHT << "\n";
                show_new_prompt = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void importPrompt() {
    if (show_import_prompt) {
        ImGui::OpenPopup("Import Image");

        IGFD::FileDialogConfig config;
        config.path = "C:/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseImportDlgKey", "Choose File", ".png", config);
    }
}

void ShowTopMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem(ICON_FA_FILE_CIRCLE_PLUS "  New")) {
                show_new_prompt = true;
            }
            if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN "  Import...")) {
                show_import_prompt = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK "  Save", "Ctrl+S")) {
            }
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_XMARK "  Exit")) {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            // Add edit options here
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools")) {
            // Add tools options here
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings")) {
            // Settings options
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            // Help section
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void savePrompt() {
    if (ImGui::Button("Save Image")) {
        std::cout << "clicky!!!\n";
        IGFD::FileDialogConfig config;
        config.path = "C:/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", config);
    }
}

void layerView() {
    std::string eye_status = ICON_FA_EYE;

    if (layers[layer_index].hidden == true) {
        eye_status = ICON_FA_EYE_LOW_VISION;    
    } else if (layers[layer_index].hidden == false) {
        eye_status = ICON_FA_EYE;
    }

    for (int i = 0; i < layers.size(); i++) {
        bool element_selected = (i == layer_index);

        if (ImGui::Selectable(layers[i].name.c_str(), element_selected)) {
            layer_index = i;
        }
    }

    if (ImGui::Button(ICON_FA_SQUARE_PLUS)) {
        createLayer();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_SQUARE_MINUS)) {
        deleteLayer(layer_index);
    }
    ImGui::SameLine();
    if (ImGui::Button(eye_status.c_str())) {
        layers[layer_index].toggle();
    }
}

// all gui elements that are used in the program
void createGuiElements() {
    // start the imgui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (show_new_prompt) {
        newImagePrompt();
    }
    
    if (show_import_prompt) {
        importPrompt();
    }
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    ShowTopMenuBar();
    createBasicGuiElement(true, {200, 200}, {float(w), 10}, ImGuiCond_Once, ImGuiCond_Once, "Color", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove, {std::bind(ImGui::ColorPicker4, "colorwheel", current_color, 0, current_color)}, 0, 1.0f);
    createBasicGuiElement(true, {130, 95}, {0, 200}, ImGuiCond_Once, ImGuiCond_Once, "Layers", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove, {std::bind(layerView)}, 0, 1.0f);
}