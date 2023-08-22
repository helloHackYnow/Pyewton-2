#include "Simulation.h"

namespace Pyewton::Frigg
{
	using namespace std::chrono_literals;

	void Simulate(std::vector<Body>* bodyList_, float simulated_duration)
	{
		std::vector<Body> & bodyList = *bodyList_;

		int nbBody = bodyList.size();

		int i = 0;
		std::for_each(std::execution::unseq, bodyList.begin(), bodyList.end(),
			[&i, &bodyList, nbBody, simulated_duration](Body& body)
			{
				//Do other bodies affect the current one
				if (body.isAffected)
				{
					glm::vec3 attraction = glm::vec3(0, 0, 0);

					for (int j = 0; j < nbBody; j++)
					{
						if (i != j && bodyList[j].affectOther)
						{
							glm::vec3 vec_ij = bodyList[j].position - body.position;
							float dst = vec_ij.length();

							glm::vec3 attraction_local = vec_ij;

							attraction_local *= (body.mass * bodyList[j].mass) / (dst * dst);
							attraction += attraction_local;
						}
					}

					//Apply the attraction
					bodyList[i].velocity += (attraction * simulated_duration) / bodyList[i].mass;
				}
				i++;
			});


		for (int i = 0; i < nbBody; i++)
		{
			if (bodyList[i].isAffected)
			{
				//Update the position

				bodyList[i].position += bodyList[i].velocity * simulated_duration;
				bodyList[i].UpdateOrbit();
			}
		}
	}


	void SimulateLoop(SimulationFunction simulate, std::vector<Body>* bodyList, float simulated_duration, std::mutex* mutex, SimulationInfos* infos)
	{
		using namespace std::chrono;

		while (infos->isSimulationRunning)
		{
			infos->mutex.lock();
			int simPerBatch = infos->simulationPerBatch;
			nanoseconds dur = infos->durationBtwBatch;

			(infos->nbIterations) += simPerBatch;
			infos->mutex.unlock();

			for (int i = 0; i < simPerBatch; i++)
			{
				mutex->lock();
				simulate(bodyList, simulated_duration);
				mutex->unlock();
			}

			std::this_thread::sleep_for(dur);
		}
	}

	void Simulator::SetModel(SimulationFunction func)
	{
		simulationModel = Frigg::Simulate;
	}

	std::unique_ptr<SystemStateHolder> Simulator::Precompute(std::vector<Body> bodyList, float simulated_duration, int N)
	{
		std::vector<Body> myBodyList = bodyList;

		SystemStateHolder* ptr = new SystemStateHolder(myBodyList.size(), N, simulated_duration);

		for (int i = 0; i < N; i++)
		{
			Frigg::Simulate(&myBodyList, simulated_duration);
			ptr->AddState(&myBodyList);
		}

		std::unique_ptr<SystemStateHolder> ptr_(ptr);

		return ptr_;
	}

	void Simulator::Start(std::vector<Body>* bodyList, float simulated_duration)
	{
		std::cout << "Starting simualtion thread ...\n";
		if (!infos.isSimulationRunning)
		{
			infos.isSimulationRunning = true;
			this->simulationWorker = std::thread(Frigg::SimulateLoop, simulationModel, bodyList, simulated_duration, &mutex, &infos);
		}
	}

	void Simulator::Stop()
	{
		if (infos.isSimulationRunning)
		{
			infos.isSimulationRunning = false;
			simulationWorker.join();
		}	
	}
}