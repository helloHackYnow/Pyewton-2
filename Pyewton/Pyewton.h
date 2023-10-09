#pragma once

#include <imgui.h>
#include "Body/Body.h"
#include "Renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <filesystem>
#include <thread>
#include "Simulation/replay.h"
#include "Simulation/Simulation.h"
#include "A3rdParty/imgui-notify/imgui_notify.h"
#include "A3rdParty/Fonts/tahoma.h"
#include "A3rdParty/ImSpinner/imspinner.h"
#include <format>
//#include "SubApps/Viewport/Viewport.h"
#include "AppData.h"
#include "SubApp/Collection.h" 
#include "AppControllers.h"
#include "JSON/Loader.h"
#include "A3rdParty/ImFileDialog/ImFileDialog.h"


namespace Pyewton
{
	

	class Application {
	public:
		Application();
		void Init(GLFWwindow* window, const char* glsl_version);
		void SetupImGuiStyle();
		void MainLoop();

		void NewFrame();
		void UpdateUI();
		void UpdateInputs();
		void UpdateTiming();
		void UpdateReplay();
		void UpdateFocusedSubAppIndex();
		void RenderUI();
		void RenderScene();
		void Shutdown();

		// All draw func
	private:
		template<typename SubApp_Type> void AddSubApps(std::string name);


		void InitSubApps();
		void DrawSubApps();

		void DrawViewport();
		void DrawBodyList();
		void DrawSimulationSettings();
		void DrawCameraParameters();
		void DrawPrecomputeDebug();
		void DrawPrecomputePlayer();
		void DrawNotifications();
		void DrawMenu();

		void SaveFunction();
		void LoadFunction();

		void SyncBodyListsStyle();

		void UpdateFileDialog();

	public:

		AppData data;
		AppControllers controllers;
		std::vector<std::unique_ptr<App::SubApp>> subApps;

	private:

		// Window manager
		bool show_viewport				= true;
		bool show_bodylist				= true;
		bool show_simulationControl		= true;
		bool show_cameraParameters		= true;
		bool show_precomputeDebug		= true;
		bool show_precomputePlayer		= true;

		//ui control
	private:
		void setViewportFocus();
	};
}