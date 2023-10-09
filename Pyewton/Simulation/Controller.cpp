#include "Controller.h"


Pyewton::Frigg::SimulationController::SimulationController(std::vector<std::unique_ptr<Pyewton::Frigg::SimulationHolder>>* parentHolder)
{
	this->parentHolder = parentHolder;

	controllerWorker = std::thread(
		[=]()
		{
			while (this->controllerRunning)
			{
				queueMutex.lock();
				if (this->simulationQueue.size() > 0)
				{
					SimulationRequest request = simulationQueue.at(0);
					simulationQueue.erase(simulationQueue.begin());
					queueMutex.unlock();

					
					ProcessRequest(request);

				}
				else
				{
					queueMutex.unlock();
				}
			}
		}
	);
}

void Pyewton::Frigg::SimulationController::Init(AppData* parentData)
{
	this->parentData = parentData;
}

Pyewton::Frigg::SimulationController::~SimulationController()
{
	controllerRunning = false;
	controllerWorker.join();
}

void Pyewton::Frigg::SimulationController::AddRequestToQueue(SimulationRequest request)
{
	queueMutex.lock();
	simulationQueue.push_back(request);
	queueMutex.unlock();
}

std::vector<Pyewton::Frigg::SimulationRequest>* Pyewton::Frigg::SimulationController::GetRequestQueue()
{
	return &simulationQueue;
}

void Pyewton::Frigg::SimulationController::ProcessRequest(SimulationRequest request)
{
	ImGui::InsertNotification({ ImGuiToastType_Info, 3000, "Launch of the simulation" });

	isPrecomputing = true;

	BodyList requestBodylist = request.systemInitialState;

	SimulationHolder* local_holder = new SimulationHolder;
	local_holder->Init(requestBodylist.size(), request.iterations);

	std::thread* local_thread = parentData->simulation.CreatePrecomputeThread(requestBodylist, request.time_btwn_iterations, request.iterations, local_holder);

	parentHolder->push_back(std::unique_ptr<SimulationHolder>(local_holder));

	local_thread->join();

	isPrecomputing = false;

	ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Finish !" });

}
