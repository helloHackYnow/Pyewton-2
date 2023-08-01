#pragma once

#include <imgui.h>
#include "Framebuffer.h"
#include "Body/Body.h"
#include "Renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <filesystem>

class Pyewton {
public:
	Pyewton();
	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	virtual void UpdateUI();
	void UpdateInputs();
	void UpdateTiming();
	void RenderUI();
	void RenderScene();
	void Shutdown();

public:
	GLFWwindow* window;
	Renderer renderer;
	std::vector<Body> bodyList;

// All draw func
private:
	void DrawViewport();
	void DrawBodyList();
	void DrawSimulationSettings();

private:
	void InitFbo();
	void rescaleFbo();

	unsigned int fboID;
	unsigned int texID;
	unsigned int rboID;

	int render_width;
	int render_height;

	// Is the user inputs are controlling the camera
	bool isUsingCamera = false;

	//Timing var 
	float deltaTime;
	float currentFrame;
	float lastFrame;

	

//ui control
private:
	void setViewportFocus();
};

