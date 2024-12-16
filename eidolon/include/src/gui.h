#pragma once
#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <functional>
#include "globals.h"
#include "imgui\imgui.h"
#include <SDL_surface.h>


// creates a basic ImGui element, can add a vector of functions to execute in order, call std::bind when giving the functions argument, supply function args in 
void createBasicGuiElement(bool set_size, ImVec2 size = { 100, 100 }, ImVec2 position = { 0, 0 }, ImGuiCond_ size_flags = ImGuiCond_None, ImGuiCond_ pos_flags = ImGuiCond_None, const char* title = "", ImGuiWindowFlags flags = 0, std::vector<std::function<void()>> functions = {}, bool* open = 0, float alpha = 1.0f) {
        
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

        SDL_Rect rect = { x0, y0, 1, 1 };
        SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, current_color[0] * 255.0f, current_color[1] * 255.0f, current_color[2] * 255.0f, current_color[3]));

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

// test function
void createSomething() {
    createBasicGuiElement(true, { 200, 200 }, { 300, 175 }, ImGuiCond_Once, ImGuiCond_Once, "test", 0, { std::bind(ImGui::Text, "hello this is a test"), std::bind(ImGui::Text, "moar text :3"), std::bind(ImGui::ColorPicker4, "colorwheel", current_color, 0, current_color)}, 0, 1.0f);
}

void savePrompt()
{
    if (ImGui::Button("Save Image")) 
    {
        std::cout << "clicky!!!\n";
        IGFD::FileDialogConfig config; config.path = "C:/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png", config);

    }
}

void importPrompt()
{
    if (ImGui::Button("Import")) 
    {
        IGFD::FileDialogConfig config; config.path = "C:/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseImportDlgKey", "Choose File", ".png", config);

    }
}

// all gui elements that are used in the program
void createGuiElements()
{
    // start the imgui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    createBasicGuiElement(true, { 200, 200 }, { 0, 0 }, ImGuiCond_Once, ImGuiCond_Once, "color", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove, { std::bind(ImGui::ColorPicker4, "colorwheel", current_color, 0, current_color) }, 0, 1.0f);

    createBasicGuiElement(true, { 100, 50 }, { 200, 0 }, ImGuiCond_Once, ImGuiCond_Once, "Save", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove, { std::bind(savePrompt) }, 0, 1.0f);

    createBasicGuiElement(true, { 150, 50 }, { 400, 0 }, ImGuiCond_Once, ImGuiCond_Once, "Import", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove, { std::bind(importPrompt) }, 0, 1.0f);

    ImGui::Begin("New", nullptr, 0);

    // input buffers
    static char width_buffer[128] = "400";
    ImGui::InputText("Width", width_buffer, IM_ARRAYSIZE(width_buffer), 0);
    static char height_buffer[128] = "500";
    ImGui::InputText("Height", height_buffer, IM_ARRAYSIZE(height_buffer), 0);

    // create new rectangle with the given sizes
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


#endif