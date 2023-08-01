#pragma once

#include <iostream>
#pragma once
#include <glad/glad.h>
#include "Shader/Shader.h"
#include "../Framebuffer.h"
#include "../Body/Body.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer {
public:
	Renderer();
	void InitRenderer(const char* vertex_path = "", const char* fragment_path = "", float width = 800, float height = 600);
	void Render(std::vector<Body>& bodyList, unsigned int fbo );

	void setViewport(float width, float height);

public:
	Camera camera;
	//Simulation
	Color ambientLight = Color(0.2);

private:
	Shader mainShader;

	float viewport_width;
	float viewport_height;
};


