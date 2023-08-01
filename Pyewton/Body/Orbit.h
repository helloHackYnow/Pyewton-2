#pragma once

#include "../Renderer/Shader/Shader.h"
#include "../Renderer/Color.h"

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

const int MAX_ORBIT_POINT = 1000;

class Orbit
{
public:
	Orbit(Shader orbitShader);
	~Orbit();

	void AppendPoint(glm::vec3 point);

	void DrawOrbit(glm::mat4 projection, glm::mat4 view, glm::mat4 model);


	

private:
	unsigned int VAO;
	unsigned int VBO;

	std::vector<glm::vec3> vertexList;

	Shader shader;
	
	Color orbitColor;

};