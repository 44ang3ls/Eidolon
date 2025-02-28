#pragma once

#include "SDL.h"
#include "imgui\ImGuiFileDialog.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl2.h"
#include "imgui\imgui_impl_sdlrenderer2.h"
#include "layers.h"
#include "src\gui.h"


unsigned int Color(unsigned int c) {
    // add alpha.
    // also swap red and blue channel for some reason.
    // todo: figure out why, and fix it.
    const short a = 0xFF;
    const short r = (c >> 16) & 0xFF;
    const short g = (c >> 8) & 0xFF;
    const short b = (c >> 0) & 0xFF;
    return (a << 24) | (r << 0) | (g << 8) | (b << 16);
}

// initialize everything we need for rendering, also creates a context for imgui and bind it to SDL2
void renderInit() {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(version_id, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_SetWindowResizable(window, SDL_bool(true));

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    drawing_surface = SDL_CreateRGBSurfaceWithFormat(0, SURFACE_WIDTH, SURFACE_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
    // SDL_FillRect(drawing_surface, NULL, SDL_MapRGB(drawing_surface->format, 255, 255, 255));

    Layer default_layer("Layer 1", 1.0, false);

    // SDL_FreeSurface(surface_example);

    layer_index = 0;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    ImFontConfig icons_config;
    ImFontConfig font_config;
    icons_config.MergeMode = true;  // Merge icon font to the previous font if you want to have both icons and text
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 16.0f);
    io.Fonts->AddFontFromMemoryCompressedTTF(FA_compressed_data, FA_compressed_size, 16.0f, &icons_config, icons_ranges); 

    ImGui_ImplSDLRenderer2_Init(renderer);
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    io.Fonts->Build();
    
    ImGui::StyleColorsDark();

    ImGuiStyle* style = &ImGui::GetStyle();
    style->GrabRounding = 4.0f;
    style->WindowRounding = 5.0f;

    ImVec4* colors = style->Colors;
    colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(Color(0x4B4B4B));  // text on hovered controls is gray900
    colors[ImGuiCol_TextDisabled] = ImGui::ColorConvertU32ToFloat4(Color(0xB3B3B3));
    colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(Color(0xF5F5F5));
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImGui::ColorConvertU32ToFloat4(Color(0xFFFFFF));  // not sure about this. Note: applies to tooltips too.
    colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(Color(0xE1E1E1));
    colors[ImGuiCol_BorderShadow] = ImGui::ColorConvertU32ToFloat4(0x00000000);  // We don't want shadows. Ever.
    colors[ImGuiCol_FrameBg] = ImGui::ColorConvertU32ToFloat4(Color(0xFAFAFA));  // this isnt right, spectrum does not do this, but it's a good fallback
    colors[ImGuiCol_FrameBgHovered] = ImGui::ColorConvertU32ToFloat4(Color(0xFFFFFF));
    colors[ImGuiCol_FrameBgActive] = ImGui::ColorConvertU32ToFloat4(Color(0xEAEAEA));
    colors[ImGuiCol_TitleBg] = ImGui::ColorConvertU32ToFloat4(Color(0xE1E1E1));  // those titlebar values are totally made up, spectrum does not have this.
    colors[ImGuiCol_TitleBgActive] = ImGui::ColorConvertU32ToFloat4(Color(0xEAEAEA));
    colors[ImGuiCol_TitleBgCollapsed] = ImGui::ColorConvertU32ToFloat4(Color(0xCACACA));
    colors[ImGuiCol_MenuBarBg] = ImGui::ColorConvertU32ToFloat4(Color(0xF5F5F5));
    colors[ImGuiCol_ScrollbarBg] = ImGui::ColorConvertU32ToFloat4(Color(0xF5F5F5));  // same as regular background
    colors[ImGuiCol_ScrollbarGrab] = ImGui::ColorConvertU32ToFloat4(Color(0xCACACA));
    colors[ImGuiCol_ScrollbarGrabHovered] = ImGui::ColorConvertU32ToFloat4(Color(0x8E8E8E));
    colors[ImGuiCol_ScrollbarGrabActive] = ImGui::ColorConvertU32ToFloat4(Color(0x707070));
    colors[ImGuiCol_CheckMark] = ImGui::ColorConvertU32ToFloat4(Color(0x1473E6));
    colors[ImGuiCol_SliderGrab] = ImGui::ColorConvertU32ToFloat4(Color(0x707070));
    colors[ImGuiCol_SliderGrabActive] = ImGui::ColorConvertU32ToFloat4(Color(0x4B4B4B));
    colors[ImGuiCol_Button] = ImGui::ColorConvertU32ToFloat4(Color(0xFAFAFA)); // match default button to Spectrum's 'Action Button'.
    colors[ImGuiCol_ButtonHovered] = ImGui::ColorConvertU32ToFloat4(Color(0xFFFFFF));
    colors[ImGuiCol_ButtonActive] = ImGui::ColorConvertU32ToFloat4(Color(0xEAEAEA));
    colors[ImGuiCol_Header] = ImGui::ColorConvertU32ToFloat4(Color(0x2680EB));
    colors[ImGuiCol_HeaderHovered] = ImGui::ColorConvertU32ToFloat4(Color(0x1473E6));
    colors[ImGuiCol_HeaderActive] = ImGui::ColorConvertU32ToFloat4(Color(0x0D66D0));
    colors[ImGuiCol_Separator] = ImGui::ColorConvertU32ToFloat4(Color(0xCACACA));
    colors[ImGuiCol_SeparatorHovered] = ImGui::ColorConvertU32ToFloat4(Color(0x8E8E8E));
    colors[ImGuiCol_SeparatorActive] = ImGui::ColorConvertU32ToFloat4(Color(0x707070));
    colors[ImGuiCol_ResizeGrip] = ImGui::ColorConvertU32ToFloat4(Color(0xCACACA));
    colors[ImGuiCol_ResizeGripHovered] = ImGui::ColorConvertU32ToFloat4(Color(0x8E8E8E));
    colors[ImGuiCol_ResizeGripActive] = ImGui::ColorConvertU32ToFloat4(Color(0x707070));
    colors[ImGuiCol_PlotLines] = ImGui::ColorConvertU32ToFloat4(Color(0x2680EB));
    colors[ImGuiCol_PlotLinesHovered] = ImGui::ColorConvertU32ToFloat4(Color(0x0D66D0));
    colors[ImGuiCol_PlotHistogram] = ImGui::ColorConvertU32ToFloat4(Color(0x2680EB));
    colors[ImGuiCol_PlotHistogramHovered] = ImGui::ColorConvertU32ToFloat4(Color(0x0D66D0));
    colors[ImGuiCol_TextSelectedBg] = ImGui::ColorConvertU32ToFloat4((Color(0x2680EB) & 0x00FFFFFF) | 0x33000000);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImGui::ColorConvertU32ToFloat4((Color(0x2C2C2C) & 0x00FFFFFF) | 0x0A000000);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void mergeLayers() {
    SDL_Rect src_rect = {0, 0, drawing_surface->w, drawing_surface->h};
    SDL_Rect dst_rect = {dst_x / 2, dst_y / 2, SURFACE_WIDTH * scroll_amt, SURFACE_HEIGHT * scroll_amt};

    for (int i = layers.size() - 1; i >= 0; i--) {
        // std::cout << i;
        if (layers[i].hidden != true) {
            SDL_BlitSurface(layers[i].layer_data, &src_rect, drawing_surface, &dst_rect);
        }
    }
}
