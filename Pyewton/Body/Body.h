#pragma once

	#include "../Files.h"
	#include "../Renderer/Model.h"
	#include "../Renderer/Mesh.h"
	#include "../Renderer/Light.h"
	#include "../Renderer/Color.h"

	#include "Orbit.h"

	#include <glm/glm.hpp>
	#include <filesystem>
	#include <vector>

namespace Pyewton
{

class Body;
typedef std::vector<Body> BodyList;

	struct BodyAppearance
	{
		Color color = Color(1);
		float size = 1.f;
		
		PointLight light;
		bool isEmissive		= false;

		Model model;
	};

	struct BodyPhysics
	{
		glm::vec3 position;
		glm::vec3 velocity;
		float mass = 10;
		bool affectOther = true;
		bool isAffected = true;
	};


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

	void UpdateOrbit();

	void ResetOrbit();

	glm::vec3 getPos();

public:
	std::string name;

	BodyAppearance appearance;

	BodyPhysics physics;

	Orbit orbit;
};

} // Namespace Pyewton