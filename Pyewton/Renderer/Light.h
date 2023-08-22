#pragma once
#include <glm/glm.hpp>
#include "Color.h"

struct PointLight {
	glm::vec3 position;
	Color color;
	float strength = 0;
};