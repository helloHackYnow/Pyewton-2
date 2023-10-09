#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Body/Body.h"
#include "replay.h"
#include "../A3rdParty/imgui-notify/imgui_notify.h"
#include <iostream>
#include <format>
#include <thread>
#include <chrono>
#include <mutex>
#include <execution>
#include <future>

namespace Pyewton::Frigg
{
	class SimulationHolder;

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

	

	typedef void(*SimulationFunction)(BodyList*, float, bool);

	void Simulate(BodyList* bodyList, float simulated_duration, bool update_orbit = false);

	void SimulateLoop(SimulationFunction simulate, BodyList* bodyList, float simulated_duration, std::mutex* mutex, SimulationInfos* infos);


	class Simulator
	{
	public:
		Simulator() = default;
		~Simulator() = default;

		void SetModel(SimulationFunction func);

		// For multithreading
		SimulationInfos infos;

		std::thread simulationWorker;
		std::thread* precomputeWorker;
		std::mutex mutex;

		//Start a thread for computing N passes
		//std::future<std::unique_ptr<SystemStateHolder>> Precompute(BodyList bodyList, float simulated_duration, int N);
		SimulationHolder* Precompute(BodyList& bodyList, float interval_duration, int N, void(*EndCallback)() = nullptr);

		std::thread* CreatePrecomputeThread(BodyList& bodylist, float intervalDuration, int N, SimulationHolder* holder, void(*EndCallback)() = nullptr);

		//Start a thread for computing an infinite number of passes
		void Start(BodyList* bodyList, float simulated_duration);
		void Stop();

	private:

		SimulationFunction simulationModel = nullptr;


	};
}