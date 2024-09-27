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
#include "imgui\imgui_impl_opengl3.h"


// creates a basic ImGui element, can add a vector of functions to execute in order 
void createBasicGuiElement(bool set_size, ImVec2 size = { 100, 100 }, ImVec2 position = { 0, 0 }, const char* title = "", ImGuiWindowFlags flags = 0, std::vector<std::function<void()>> functions = {}, bool* open = 0, float alpha = 1.0f) {
        
    if (set_size) {
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(position);
        ImGui::SetNextWindowBgAlpha(alpha);
    }

    ImGui::Begin(title, nullptr, flags);

    if (functions.size() != 0) {
        for (int i = 0; i < functions.size(); i++) {
            functions[i]();
        }
    }

    ImGui::End();
}

void createSomething() {
    createBasicGuiElement(false, { 200, 200 }, { 300, 175 }, "test", 0, { std::bind(ImGui::Text, "hello this is a test"), std::bind(ImGui::Text, "moar text :3")}, 0, 1.0f);
}

class GuiHandler {

    void drawRgbWheel() {

        ImGui::Begin("Fixed Window", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::ColorEdit4("color wheel", color);
        ImGui::End();
    }
};



#endif