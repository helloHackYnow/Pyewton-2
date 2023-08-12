#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Body/Body.h"
#include <thread>
#include <chrono>
#include <mutex>


namespace Pyewton::Frigg
{
	typedef void(*SimulationFunction)(std::vector<Body>*, float);

	void Simulate(std::vector<Body>* bodyList, float simulated_duration);

	// Simulate the system behaviour for N passes.
	// Basically run the simulation N times.
	//
	// N must be a positive integer
	void SimulateN(SimulationFunction simulate, std::vector<Body>* bodyList, float simulated_duration, int N, std::mutex* bodyList_access);

	void SimulateLoop(SimulationFunction simulate, std::vector<Body>* bodyList, float simulated_duration, std::mutex* bodyList_access);


	// Launch a thread for computing the 
	std::thread LaunchSimulationThread(SimulationFunction simulate, std::vector<Body>* bodyList, float simulated_duration, int N, std::mutex* bodyList_access);
}