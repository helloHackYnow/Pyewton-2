#pragma once

#include "Body/Body.h"
#include "Renderer/Renderer.h"

struct AppData
{
	GLFWwindow* window = nullptr;
	Pyewton::Odin::Renderer renderer;
	Pyewton::BodyList displaySystem;
	Pyewton::BodyList editableSystem;

	std::mutex bodyList_access;

	int nb_iteration = 0;

	int render_width = 0;
	int render_height = 0;

	// Is the user inputs are controlling the camera
	bool isUsingCamera = false;

	//Timing var 
	float deltaTime = 1.f;
	float currentFrame = 1.f;
	float lastFrame = 1.f;

	bool isSimulationRunning = false;
	Pyewton::Frigg::Simulator simulation;

	bool isPrecomputing = false;

	//Hold all the simulations holders pointers
	std::vector<std::unique_ptr<Pyewton::Frigg::SimulationHolder>> holders;


	Pyewton::Frigg::ReplayControler player;
	bool isReplaying = false;
	bool isHolderLoaded = false;
	int loadedHolderIndex = 0;

	bool is_saving = false;
	bool is_opening = false;

	int focused_subApp_index = -1; //index of the currently focused subApp, -1 means none are focused

};