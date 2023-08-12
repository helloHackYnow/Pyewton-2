#pragma once

#include <imgui.h>
#include "Body/Body.h"
#include "Renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <filesystem>
#include "Simulation/Simulation.h"

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
		void RenderUI();
		void RenderScene();
		void Shutdown();

	public:
		GLFWwindow* window;
		Odin::Renderer renderer;
		std::vector<Body> bodyList;

		std::mutex bodyList_access;

		// All draw func
	private:
		void DrawViewport();
		void DrawBodyList();
		void DrawSimulationControl();
		void DrawSimulationSettings();
		void DrawCameraParameters();

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
		std::thread simulationWorker;


		//ui control
	private:
		void setViewportFocus();
	};
}