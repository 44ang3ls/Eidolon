#pragma once

#ifndef LAYERS_H
#define LAYERS_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <functional>
#include <SDL_surface.h>
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
		//SDL_FillRect(layer_data, NULL, SDL_MapRGB(layer_data->format, 150, 0, 0));

		name = n;
		opacity_percent = o;
		hidden = h;


		layers.push_back(*this);
	}

};


void clearLayers()
{
	layers.erase(layers.begin(), layers.end());
}

//std::vector<Layer*> layers;

#endif