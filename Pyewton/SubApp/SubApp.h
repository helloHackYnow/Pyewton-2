#pragma once
#include "../AppData.h"
#include "imgui.h"
#include "string"

namespace Pyewton::App {}

//class Pyewton::App::BodyList;




namespace Pyewton
{
	namespace App
	{


	class SubApp {
	public:
		SubApp() = default;

		void Init(AppData* appdata)
		{
			parentAppData = appdata;
		}

		virtual void Draw() = 0;
		virtual void ProcessInputs() {};
		// ImGui draw function

	public:
		bool is_focused = false;
		bool was_focused = false;
		bool is_open;
		struct AppData* parentAppData;
		std::string name;
	};

	} // Namespace App
} // Namespace Pyewton