#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include <glad.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl2.h"
#include "imgui\imgui_impl_opengl3.h"

#include "src/gui.h"

#undef main

const char* version_designation = " Prototype ";
//const char* version_id = "0.0.1a";

char full_name[32];


int main(int, char**) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(version_id, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "failed to init glad \n";
        return -1;
    }

    // Setup ImGui context
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 130"); // Adjust for your OpenGL version

    SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED);

    SDL_GL_MakeCurrent(window, glContext);


    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Your ImGui code here
        ImGui::Begin("hiiii");
        ImGui::Text("hi");
        ImGui::End();

        ImGui::Begin("2");
        ImGui::Text("177013");
        ImGui::End();

        ImGui::Begin("iiiiiiiiiiiiii");
        ImGui::Text("what the what the");
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once); // Fixed position
        ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_Once); // Fixed size
        ImGui::Begin("Fixed Window", nullptr, 0);

        // Window contents
        ImGui::Text("Fixed pos");
        ImGui::Button("A Button", ImVec2(100, 30));
        static char buffer[128] = "input text";
        ImGui::InputText("inp", buffer, IM_ARRAYSIZE(buffer), 0);
        static float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

        ImGui::ColorEdit4("color wheel", color);

        ImGui::End();


        createSomething();

        // Render
        ImGui::Render();
        SDL_GL_MakeCurrent(window, glContext);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Set clear color
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}