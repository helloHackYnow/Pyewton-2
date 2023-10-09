#ifndef PRECOMPUTE_QUEUE_WINDOW_H
#define PRECOMPUTE_QUEUE_WINDOW_H

#include "SubApp.h"
#include "../Simulation/Controller.h"
#include "../Simulation/Simulation.h"

namespace Pyewton::App
{


class Queue : SubApp
{
public:
	using SubApp::SubApp;

	void Draw()
	{
		if (ImGui::Begin("Request queue", is_open))
		{
			std::vector<Pyewton::

			for (auto & request : )
			{

			}

			ImGui::End();
		}
	}
};

}
#endif