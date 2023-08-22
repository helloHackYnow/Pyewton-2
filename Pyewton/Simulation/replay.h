#pragma once

#include "Precompute/Precompute.h"
#include "Simulation.h"

namespace Pyewton::Frigg
{
	class Player
	{
	public:
		Player() = default;
		~Player() = default;

		void SetBodyList(std::vector<Body> list);

		void SetHolder(SystemStateHolder* holder);
		void SetSpeed(float speed);

		void ResetReplayPos();

		void StartReplay();
		void Next();

		std::vector<Body> displayBodyList;
		float replaySpeed = 1;
		int replayPos = 0;
	private:

		SystemStateHolder* holder;


	};
}
