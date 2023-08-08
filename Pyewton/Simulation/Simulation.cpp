#include "Simulation.h"



void Simulate(std::vector<Body>& bodyList, float simulated_duration)
{
	int nbBody = bodyList.size();

	for (int i = 0; i < nbBody; i++)
	{
		//Do other bodies affect the current one
		if (bodyList[i].isAffected)
		{
			glm::vec3 attraction = glm::vec3(0, 0, 0);

			for (int j = 0; j < nbBody; j++)
			{
				if (i != j && bodyList[j].affectOther)
				{
					glm::vec3 vec_ij = bodyList[j].position - bodyList[i].position;
					float dst = vec_ij.length();

					glm::vec3 attraction_local = vec_ij;

					attraction_local *= (bodyList[i].mass * bodyList[j].mass) / (dst * dst);
					attraction += attraction_local;
				}
			}

			//Apply the attraction
			bodyList[i].velocity += (attraction * simulated_duration) / bodyList[i].mass;
		}

	}

	for (int i = 0; i < nbBody; i++)
	{
		if (bodyList[i].isAffected)
		{
			//Update the position

			bodyList[i].position += bodyList[i].velocity * simulated_duration;
			bodyList[i].orbit.AppendPoint(bodyList[i].position);
		}
	}
}

void SimulateN(std::vector<Body>& bodyList, float simulated_duration, int N)
{
	for (int i = 0; i < N; i++)
	{
		Simulate(bodyList, simulated_duration);
	}
}
