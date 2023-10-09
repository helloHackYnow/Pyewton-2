#ifndef SIMULATION_CONTROLLER_H
#define SIMULATION_CONTROLLER_H

#include "../Body/Body.h"
#include "Simulation.h"
#include "../AppData.h"

#include <thread>
#include <vector>
#include <mutex>


namespace Pyewton::Frigg
{
	struct SimulationRequest
	{
		int iterations;
		float time_btwn_iterations;

		BodyList systemInitialState;
	};

	class SimulationController
	{
	public:
		SimulationController(std::vector<std::unique_ptr<Pyewton::Frigg::SimulationHolder>>* parentHolder);
		void Init(AppData* parentData);
		~SimulationController();

		void AddRequestToQueue(SimulationRequest request);

		std::vector<SimulationRequest>* GetRequestQueue();


	private:
		void ProcessRequest(SimulationRequest request);


		AppData* parentData; // Pointer to the appdata of the main Pyewton app
		std::vector<std::unique_ptr<Pyewton::Frigg::SimulationHolder>>* parentHolder;
		std::thread controllerWorker;
		std::vector<SimulationRequest> simulationQueue;
		std::mutex queueMutex;
		bool controllerRunning = true;
		bool isPrecomputing = false;

	};

}
#endif // !SIMULATION_CONTROLLER_H
