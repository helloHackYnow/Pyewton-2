#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Body/Body.h"

void Simulate(std::vector<Body>* bodyList, float simulated_duration);