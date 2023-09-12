#include "replay.h"

namespace Pyewton::Frigg
{
	void ReplayControler::SetBodyList(BodyList list)
	{
		displayBodyList = list;
	}

	void ReplayControler::SetHolder(SimulationHolder* holder)
	{
		this->holder = holder;
	}

	void ReplayControler::SetSpeed(float speed)
	{
		this->replaySpeed = speed;
	}

	void ReplayControler::ResetReplayPos()
	{
		this->replayPos = 0;
	}

	void ReplayControler::StartReplay()
	{
		replayPos = 0;
	}

	void ReplayControler::Next()
	{
		if (replayPos < holder->lastIndex)
		{
			replayPos += 1 * replaySpeed;
			holder->PasteState(displayBodyList, replayPos);
			for (auto& body : displayBodyList)
			{
				body.orbit.AppendPoint(body.physics.position);
			}
		}
	}

	SimulationHolder::~SimulationHolder()
	{
		delete[] data;
	}

	void SimulationHolder::Init(int bodyListSize, int simulationLenght)
	{
		data = new glm::vec3[bodyListSize * simulationLenght + bodyListSize];
		lastIndex = 0;
		maxIndex = simulationLenght - 1;
		systemSize = bodyListSize;
	}

	void SimulationHolder::AppendState(BodyList& bodyList)
	{
		if (lastIndex < maxIndex)
		{
			lastIndex++;

			int i = 0;
			for(auto &body : bodyList)
			{
				data[lastIndex * systemSize + i] = body.getPos();

				i++;
			}
		}
	}

	void SimulationHolder::PasteState(BodyList& bodyList, int index)
	{	
		int i = 0;
		for (auto &body : bodyList)
		{
			int stateIndex = index * systemSize;
			body.setPos(data[stateIndex + i]);

			i++;
		}
	}

	void SimulationHolder::PasteState(BodyList& bodyList, float index)
	{
		int i = 0;
		for (auto& body : bodyList)
		{
			int stateIndex1 = (int)index * systemSize;
			int stateIndex2 = ((int)index + 1) * systemSize;

			body.setPos(glm::mix(data[stateIndex1 + i], data[stateIndex2 + i], index - (int)index));

			i++;
		}
	}

	void SimulationHolder::SetVelocity(BodyList& bodyList)
	{
		int i = 0;
		int velocityIndex = systemSize * (lastIndex + 1);
		for (auto & body : bodyList)
		{
			data[velocityIndex + i] = body.physics.velocity;

			i++;
		}
	}

	void SimulationHolder::PasteVelocity(BodyList& bodyList)
	{
		int velocityIndex = systemSize * (lastIndex + 1);
		int i = 0;
		for (auto &body: bodyList)
		{
			body.physics.velocity[velocityIndex + i];

			i++;
		}

	}

}