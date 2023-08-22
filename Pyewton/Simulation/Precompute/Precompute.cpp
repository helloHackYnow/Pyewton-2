#include "Precompute.h"

namespace Pyewton::Frigg
{
	SystemStateHolder::SystemStateHolder()
	{
	}

	SystemStateHolder::SystemStateHolder(int SystemSize, int HolderSize, float durationPerState)
	{
		this->size = SystemSize * HolderSize;
		this->data = new glm::vec3[size];

		back_pos = 0;
		back_state = 0;
		systemSize = SystemSize;
		this->durationPerState = durationPerState;
	}

	SystemStateHolder::~SystemStateHolder()
	{
		std::cout << "destructor called\n";
		delete[] this->data;
	}

	void SystemStateHolder::AddState(std::vector<Body>* bodyList)
	{
		if (back_state * systemSize < size)
		{
			for (auto& body : *bodyList)
			{
				data[back_pos] = body.getPos();
				back_pos++;
			}

			back_state++;
		}
	}

	void SystemStateHolder::Clear()
	{
		back_pos = 0;
		back_state = 0;
	}
	void SystemStateHolder::PasteState(std::vector<Body>* bodyList, int index)
	{
		if (index <= back_state) // Verifie if the index isn't greater than the last state added and the system is the correct one
		{
			int i = systemSize * index;

			for (auto& body : *bodyList)
			{
				body.position = data[i];
				i++;
			}
		}
		
	}
}