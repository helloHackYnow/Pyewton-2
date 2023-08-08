#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Body/Body.h"

void Simulate(std::vector<Body>& bodyList, float simulated_duration);

// Simulate the system behaviour for N passes.
// Basically run the simulation N times.
//
// N must be a positive integer
void SimulateN(std::vector<Body>& bodyList, float simulated_duration, int N);