#pragma once

#include "Pyewton.h"
#include "Body/Body.h"
#include "Renderer/Renderer.h"
#include "Simulation/replay.h"
#include "Simulation/Simulation.h"

namespace Pyewton
{

struct AppData
{
	GLFWwindow* window = nullptr;
	Odin::Renderer renderer;
	BodyList displaySystem;
	BodyList editableSystem;

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
	Frigg::Simulator simulation;

	bool isPrecomputing = false;

	//Hold all the simulations holders pointers
	std::vector<std::unique_ptr<Frigg::SimulationHolder>> holders;


	Frigg::ReplayControler player;
	bool isReplaying = false;
	bool isHolderLoaded = false;
	int loadedHolderIndex = 0;
};

}