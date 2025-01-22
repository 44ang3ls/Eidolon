#pragma once


#include <iostream>
#include <stdio.h>
#include <vector>
#include <functional>
#include "SDL.h"


struct Tool;

// full collection of tools
std::vector<Tool*> tools;


struct Tool {
	// the action to be executed on press, could be drawing pixels or erasing them, after every action the new layer should be put in the undo buffer
	std::function<void>* action;


	Tool(std::function<void>* a) {
		action = a;

		tools.push_back(this);
	}
};