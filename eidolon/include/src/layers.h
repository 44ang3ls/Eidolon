#pragma once

#include <SDL_surface.h>
#include <stdio.h>

#include <functional>
#include <iostream>
#include <vector>

#include "globals.h"

struct Layer;

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
};


static int counter = 0;
void createLayer()
{
    Layer new_layer(layers[layers.size() - 1].name + " " + std::to_string(counter++), 1.0, false);
}

void deleteLayer(int ind)
{
    layers.erase(layers.begin() + ind);
    layer_index = 0;
}

void clearLayers() {
    layers.erase(layers.begin(), layers.end());
}
