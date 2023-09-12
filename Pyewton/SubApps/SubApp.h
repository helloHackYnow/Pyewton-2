#pragma once

#include <string>
#include "../AppData.h"


namespace Pyewton
{
	class SubApp
	{
	public:
		SubApp();

		void Init(AppData* appdata);

		virtual void Draw(); // ImGui draw function

	public:
		bool is_open;
		struct AppData* parentAppData;
		std::string name;
	};

}