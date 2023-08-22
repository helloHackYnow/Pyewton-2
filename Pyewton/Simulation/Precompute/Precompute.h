#pragma once

#include "../../Body/Body.h"
#include <array>
#include <glm/glm.hpp>
#include <mutex>

/*
* Data structure for storing list of system state
* Data stored in this form : 
* data[] = { bodypos1, bodypos2, ..., bodyposN, bodypos1, bodypos2, ..., bodyposN, ... }
*			 |---- system state 1 -----------|	|---- system state 2 -----------|
*/

namespace Pyewton::Frigg
{

	class SystemStateHolder
	{
	public:
		SystemStateHolder();
		SystemStateHolder(int SystemSize, int HolderSize, float durationPerState);
		~SystemStateHolder();

		void AddState(std::vector<Body>* bodyList);
		void Clear();
		void PasteState(std::vector<Body>* bodyList, int index);



	public:
		glm::vec3* data;
		int size;

		// Nb of body in the simulation storage
		int systemSize = 0;

		//Index of the last system state in the array
		int back_state = 0;

		//Index of the last body position in the array
		int back_pos = 0;

		//Duration each state is simulated for
		float durationPerState;
	};
}
