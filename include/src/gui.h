#pragma once
#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <functional>
#include "globals.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl2.h"
#include "imgui\imgui_impl_sdlrenderer2.h"


// creates a basic ImGui element, can add a vector of functions to execute in order 
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

// test function
void createSomething() {
    createBasicGuiElement(true, { 200, 200 }, { 300, 175 }, ImGuiCond_Once, ImGuiCond_Once, "test", 0, { std::bind(ImGui::Text, "hello this is a test"), std::bind(ImGui::Text, "moar text :3"), std::bind(ImGui::ColorPicker4, "colorwheel", current_color, 0, current_color)}, 0, 1.0f);
}

class GuiHandler {

    void drawRgbWheel() {

        ImGui::Begin("Fixed Window", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::ColorEdit4("color wheel", current_color);
        ImGui::End();
    }
};



#endif