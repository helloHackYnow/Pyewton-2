#pragma once

#include <imgui.h>
#include "Body/Body.h"
#include "Renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <filesystem>
#include "Simulation/replay.h"
#include "Simulation/Simulation.h"
#include "Simulation/Precompute/Precompute.h"
#include "A3rdParty/imgui-notify/imgui_notify.h"
#include "A3rdParty/Fonts/tahoma.h"
#include <format>

namespace Pyewton
{
	class Application {
	public:
		Application();
		void Init(GLFWwindow* window, const char* glsl_version);
		void MainLoop();

		void NewFrame();
		void UpdateUI();
		void UpdateInputs();
		void UpdateTiming();
		void UpdateReplay();
		void RenderUI();
		void RenderScene();
		void Shutdown();

	public:
		GLFWwindow* window;
		Odin::Renderer renderer;
		std::vector<Body> bodyList;

		std::mutex bodyList_access;

		int nb_iterations;

		// All draw func
	private:
		void DrawViewport();
		void DrawBodyList();
		void DrawSimulationControl();
		void DrawSimulationSettings();
		void DrawCameraParameters();
		void DrawMultithreadingDebug();
		void DrawPrecomputeDebug();
		void DrawPrecomputePlayer();
		void DrawNotifications();

	private:

		int render_width;
		int render_height;

		// Is the user inputs are controlling the camera
		bool isUsingCamera = false;

		//Timing var 
		float deltaTime;
		float currentFrame;
		float lastFrame;

	private: //Simulation Control
		bool isSimulationRunning;
		Frigg::Simulator simulation;
		
		//Hold all the simulations holders pointers
		std::vector<std::unique_ptr<Frigg::SystemStateHolder>> holders;
		

		Frigg::Player player;
		bool isReplaying = false;
		bool isHolderLoaded = false;
		int loadedHolderIndex;

		//ui control
	private:
		void setViewportFocus();
	};
}