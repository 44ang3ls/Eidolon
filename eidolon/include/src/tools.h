#pragma once


#include <iostream>
#include <stdio.h>
#include <vector>
#include <functional>


struct Tool;

// full collection of tools
std::vector<Tool*> tools;

struct Tool {
	// the action to be executed on press, could be drawing pixels or erasing them
	std::function<void>* action;


	Tool(std::function<void>* a) {
		action = a;

		tools.push_back(this);
	}
};