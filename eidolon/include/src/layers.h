#pragma once

#include <SDL_surface.h>
#include <stdio.h>

#include <functional>
#include <iostream>
#include <vector>
#include <stack>

#include "globals.h"

struct Layer;
struct LayerBuffDat;

// collection of all layers in an image
std::vector<Layer> layers;

struct Layer {
    SDL_Surface* layer_data;
    // by default layers will be given a simple name like "Layer 1" but this can be changed
    std::string name;
    float opacity_percent;

    // if a layer if hidden it will be excluded from the rendering process, by default should be false
    bool hidden;

    void toggle() {
        hidden = !hidden;
    }

    // on creation of a layer create a blank sdl surface
    Layer(std::string n, float o, bool h) {
        layer_data = SDL_CreateRGBSurfaceWithFormat(0, SURFACE_WIDTH, SURFACE_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);

        name = n;
        opacity_percent = o;
        hidden = h;

        // push to layer vector to be used later
        std::cout << "added to layers \n";
        layers.push_back(*this);
    }

    Layer() = default;
};

struct LayerBuffDat {
    Layer layer_dat;
    int index;

    LayerBuffDat(Layer layr, int i) 
    {
        layer_dat = layr;
        index = i;
    }
};

std::stack<LayerBuffDat> layer_buffer;

static int counter = 0;
void createLayer()
{
    Layer new_layer(layers[layers.size() - 1].name + " " + std::to_string(counter++), 1.0, false);
}

void deleteLayer(int ind)
{
    if (layers.size() > 1) {
        layers.erase(layers.begin() + ind);
        layer_index = 0;
    }
}

void clearLayers() {
    layers.erase(layers.begin(), layers.end());
}

void addToBuff(int ind) {
    std::cout << "adding to buffer \n";
    layer_buffer.push(LayerBuffDat(layers[ind], ind));
}

void clearBuffPop() {
    layer_buffer.pop();
}

void undo() {

    SDL_Rect src_rect = {0, 0, layers[layer_buffer.top().index].layer_data->w, layers[layer_buffer.top().index].layer_data->h};
    SDL_Rect dst_rect = {dst_x / 2, dst_y / 2, SURFACE_WIDTH * scroll_amt, SURFACE_HEIGHT * scroll_amt};

    if (!layer_buffer.empty()) {
        SDL_FillRect(layers[layer_buffer.top().index].layer_data, NULL, 0);
        SDL_BlitSurface(layer_buffer.top().layer_dat.layer_data, &src_rect, layers[layer_buffer.top().index].layer_data, &dst_rect);  // restore the last state
        layer_buffer.pop();
        std::cout << "Undo successful. Number of layers: " << layers.size() << "\n";
    } else {
        std::cout << "Nothing to undo.\n";
    }
}