#pragma once

#include "../SubApp.h"


namespace Pyewton
{
	class Viewport : public SubApp
	{
	public:
		using SubApp::SubApp;


		void Draw() override
		{
			std::cout << "overrided\n";
		}
			/*
		{
			ImGuiWindowFlags window_flag = 0;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

			if (!ImGui::Begin("Dear ImGui Demo", &is_open, window_flag))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
			}
			else
			{
				ImGui::BeginChild("GameRender");

				// Get the size of the child (i.e. the whole draw size of the windows).
				ImVec2 wsize = ImGui::GetWindowSize();
				ImVec2 wpos = ImGui::GetWindowPos();

				parentAppData->render_width = wsize.x;
				parentAppData->render_height = wsize.y;
				//reste viewport for the renderer
				parentAppData->renderer.setViewport(parentAppData->render_width, parentAppData->render_height);
				ImGui::Image((ImTextureID)parentAppData->renderer.GetTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

				ImGui::EndChild();
				ImGui::End();
			}

			ImGui::PopStyleVar();
		}
			*/
	};
}