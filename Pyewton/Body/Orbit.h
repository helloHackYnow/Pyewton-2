#pragma once

#include "../Renderer/Shader/Shader.h"
#include "../Renderer/Color.h"

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

const int MAX_ORBIT_POINT = 10000;

class Orbit
{
public:
	Orbit();

	void AppendPoint(glm::vec3 point);

	void Draw(Shader shader);

	void SetColor(Color color);

	void Reset();


	std::vector<glm::vec3> vertexList;
	

private:
	unsigned int VAO;
	unsigned int VBO;
	
	Color orbitColor;

};