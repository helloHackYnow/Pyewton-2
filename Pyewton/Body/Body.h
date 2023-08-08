#pragma once

#include "../Renderer/Model.h"
#include "../Renderer/Mesh.h"
#include <filesystem>
#include "../Renderer/Light.h"
#include <glm/glm.hpp>
#include "../Renderer/Color.h"
#include "Orbit.h"
class Body
{
public:
	Body();

	Body(const char * name);

	void setColor(glm::vec3 colour);
	void setColor(float r, float g, float b);

	void setPos(glm::vec3 pos);
	void setPos(float x, float y, float z);

	void setLight(Color c, float strenght);

	void Draw(Shader shader);

	void ResetOrbit();

	glm::vec3 getPos();

public:
	//Cosmetic
	Color color = Color(1);
	std::string name;
	float size = 1.f;

	glm::vec3 position;
	glm::vec3 velocity;
	float mass = 10;

	PointLight light;

	bool isEmissive = false;

	bool affectOther = true;
	bool isAffected = true;

	Orbit orbit;

private:
	Model model;
};
