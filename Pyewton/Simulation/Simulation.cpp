#include "Simulation.h"



void Simulate(std::vector<Body>* bodyList, float simulated_duration)
{
	std::vector<Body>& list = (*bodyList);
	int nbBody = list.size();

	for (int i = 0; i < nbBody; i++)
	{
		//Do other bodies affect the current one
		if (list[i].isAffected)
		{
			glm::vec3 attraction = glm::vec3(0, 0, 0);

			for (int j = 0; j < nbBody; j++)
			{
				if (i != j && list[j].affectOther)
				{
					glm::vec3 vec_ij = list[j].position - list[i].position;
					float dst = vec_ij.length();

					glm::vec3 attraction_local = vec_ij / dst;

					attraction_local *= (list[i].mass * list[j].mass) / dst;
					attraction += attraction_local;
				}
			}

			//Apply the attraction
			list[i].velocity += (attraction * simulated_duration) / list[i].mass;
		}

	}

	for (int i = 0; i < nbBody; i++)
	{
		if (list[i].isAffected)
		{
			//Update the position

			list[i].position += list[i].velocity * simulated_duration;
			list[i].orbit.AppendPoint(list[i].position);
		}
	}
}
