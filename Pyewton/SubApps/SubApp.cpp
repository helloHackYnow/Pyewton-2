#include "SubApp.h"


using namespace Pyewton;


Pyewton::SubApp::SubApp() : parentAppData(nullptr), is_open(true), name("Hello")
{
}

void Pyewton::SubApp::Init(AppData* appdata)
{
	parentAppData = appdata;
}

void Pyewton::SubApp::Draw()
{
	if (ImGui::Begin(name.data(), &is_open))
	{
		ImGui::End();
	}
}
