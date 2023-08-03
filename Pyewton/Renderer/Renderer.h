#pragma once

#include <iostream>
#include <glad/glad.h>
#include "Framebuffer.h"
#include "../Body/Body.h"
#include "Camera.h"
#include "Shader/Graphics.h"
#include "Shader/Compute.h"
#include "PostProcessing/PostProcess.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer {
public:
	Renderer();
	void InitRenderer(int width = 800, int height = 600);

	//Return the index of the shader in renderer's shader list
	int AddShader(const char* vertex_path, const char* fragment_path);
	void SetShaderFlag(int shaderIndex, ShaderFlags flag);

	void Render(std::vector<Body>& bodyList);

	void setViewport(int width, int height);

	// Set in all shaders the camera related uniforms
	void UpdateShaders();

	unsigned int GetTexture();

public:
	Camera camera;
	//Simulation
	Color ambientLight = Color(0.2);

	//Index of the main shader in the shader list
	int mainShader;

	//Index of the orbit shader in the shader list
	int orbitShader;

	//List of pointers to the different shader
	std::vector<std::unique_ptr<Shader>> shaderList;
private:
	void UpdateLights(std::vector<Body>& bodyList);

private:
	Effect bloom;

	FrameBuffer fbo_internal;
	FrameBuffer fbo_output;

	//List of light points
	std::vector<PointLight> lightPointList;

	//List of shader pointer

	int viewport_width;
	int viewport_height;
};


