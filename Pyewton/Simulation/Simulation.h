#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Body/Body.h"
#include "Precompute/Precompute.h"
#include <iostream>
#include <format>
#include <thread>
#include <chrono>
#include <mutex>
#include <execution>
#include <future>


namespace Pyewton::Frigg
{


	using namespace std::chrono;
	using namespace std::chrono_literals;

	struct SimulationInfos
	{
		int nbIterations = 0;
		float targetFrequency = 60;
		int simulationPerBatch = 1;
		
		nanoseconds durationBtwBatch = 250ns;

		bool isSimulationRunning = false;
		std::mutex mutex;

		//Timing
		/*
		time_point<steady_clock> firstBatchTime;
		time_point<steady_clock> lastBatchTime;

		duration<nanoseconds> lastBatchDuration;
		*/
	};

	

	typedef void(*SimulationFunction)(std::vector<Body>*, float);

	void Simulate(std::vector<Body>* bodyList, float simulated_duration);

	void SimulateLoop(SimulationFunction simulate, std::vector<Body>* bodyList, float simulated_duration, std::mutex* mutex, SimulationInfos* infos);


	class Simulator
	{
	public:
		Simulator() = default;
		~Simulator() = default;

		void SetModel(SimulationFunction func);

		// For multithreading
		SimulationInfos infos;

		std::thread simulationWorker;
		std::mutex mutex;

		//Start a thread for computing N passes
		std::unique_ptr<SystemStateHolder> Precompute(std::vector<Body> bodyList, float simulated_duration, int N);

		//Start a thread for computing an infinite number of passes
		void Start(std::vector<Body>* bodyList, float simulated_duration);
		void Stop();

	private:

		SimulationFunction simulationModel = nullptr;


	};
}