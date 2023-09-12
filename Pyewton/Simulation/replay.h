#pragma once

#include "Simulation.h"
#include <stdexcept>

namespace Pyewton::Frigg
{
	class SimulationHolder
	{
	public:
		SimulationHolder() = default;
		~SimulationHolder();

		void Init(int bodyListSize, int simulationLenght);
		void AppendState(BodyList& bodyList);
		void PasteState(BodyList& bodyList, int index); //Without lerp
		void PasteState(BodyList& bodyList, float index); // With lerp
		void SetVelocity(BodyList& bodyList);
		void PasteVelocity(BodyList& bodyList);

	public:
		glm::vec3* data;
		int lastIndex;
		int maxIndex;
		int systemSize;

		bool isPrecomputing = false;
	};

	class ReplayControler
	{
	public:
		ReplayControler() = default;
		~ReplayControler() = default;

		void SetBodyList(BodyList list);

		void SetHolder(SimulationHolder* holder);
		void SetSpeed(float speed);

		void ResetReplayPos();

		void StartReplay();
		void Next();

		BodyList displayBodyList;
		float replaySpeed = 1;
		float replayPos = 0;
	private:

		SimulationHolder* holder;
	};
}
