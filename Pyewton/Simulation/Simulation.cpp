#include "Simulation.h"

namespace Pyewton::Frigg
{
	using namespace std::chrono_literals;

	void Simulate(BodyList* bodyList_, float simulated_duration, bool update_orbit)
	{
		BodyList & bodyList = *bodyList_;

		int nbBody = bodyList.size();

		int i = 0;
		std::for_each(std::execution::unseq, bodyList.begin(), bodyList.end(),
			[&i, &bodyList, nbBody, simulated_duration](Body& body)
			{
				//Do other bodies affect the current one
				if (body.physics.isAffected)
				{
					glm::vec3 attraction = glm::vec3(0, 0, 0);

					for (int j = 0; j < nbBody; j++)
					{
						if (i != j && bodyList[j].physics.affectOther)
						{
							glm::vec3 vec_ij = bodyList[j].physics.position - body.physics.position;
							float dst = vec_ij.length();

							glm::vec3 attraction_local = vec_ij;

							attraction_local *= (body.physics.mass * bodyList[j].physics.mass) / (dst * dst);
							attraction += attraction_local;
						}
					}

					//Apply the attraction
					bodyList[i].physics.velocity += (attraction * simulated_duration) / bodyList[i].physics.mass;
				}
				i++;
			});

		for (int i = 0; i < nbBody; i++)
		{
			if (bodyList[i].physics.isAffected)
			{
				//Update the position

				bodyList[i].physics.position += bodyList[i].physics.velocity * simulated_duration;
				if (update_orbit)
				{
					bodyList[i].UpdateOrbit();
				}
			}
		}
	}


	void SimulateLoop(SimulationFunction simulate, BodyList* bodyList, float simulated_duration, std::mutex* mutex, SimulationInfos* infos)
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
				simulate(bodyList, simulated_duration, false);
				mutex->unlock();
			}

			std::this_thread::sleep_for(dur);
		}
	}

	void Simulator::SetModel(SimulationFunction func)
	{
		simulationModel = Frigg::Simulate;
	}

	SimulationHolder* Simulator::Precompute(BodyList& bodyList, float interval_duration, int N, void(*EndCallback)())
	{
		SimulationHolder *holder = new SimulationHolder;

		holder->Init(bodyList.size(), N);

		SimulateInHolder(bodyList, interval_duration, N, holder, EndCallback);

		return holder;
	}

	void Simulator::SimulateInHolder(BodyList &bodylist, float intervalDuration, int N, SimulationHolder* holder, void(*EndCallback)())
	{
		BodyList *my_BodyList = &bodylist;

		precomputeWorker = std::thread(
			[=]() {
				holder->isPrecomputing = true;
				for (int i = 0; i < N; i++)
				{
					Frigg::Simulate(my_BodyList, intervalDuration);
					holder->AppendState(*my_BodyList);
				}

				//holder->PasteVelocity(*my_BodyList);
				holder->isPrecomputing = false;
				EndCallback();
			}
		);
	}

	void Simulator::Start(BodyList* bodyList, float simulated_duration)
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