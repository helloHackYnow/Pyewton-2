#pragma once

#include "SubApp.h"

namespace Pyewton
{
	namespace App
	{
		class Viewport : public SubApp
		{
		public:
			using SubApp::SubApp;

			void Draw() override
			{
				ImGuiWindowFlags window_flag = 0;

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

				if (!ImGui::Begin(name.data(), &is_open, window_flag))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
				}
				else
				{
					is_focused = ImGui::IsWindowFocused();
					
					ImGui::BeginChild("GameRender");

					if (!is_focused && ImGui::IsWindowFocused())
					{
						is_focused = true;
					}

					if (is_focused)
					{
						glfwSetInputMode(parentAppData->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
					}
					else
					{
						glfwSetInputMode(parentAppData->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}

					// Get the size of the child (i.e. the whole draw size of the windows).
					ImVec2 wsize = ImGui::GetWindowSize();
					ImVec2 wpos = ImGui::GetWindowPos();

					viewportCenter.x = wpos.x + wsize.x / 2;
					viewportCenter.y = wpos.y + wsize.y / 2;

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

			void ProcessMouse()
			{
				static double lastX = 0;
				static double lastY = 0;
				static bool firstFrame = true;

				double mousex;
				double mousey;

				glfwGetCursorPos(parentAppData->window, &mousex, &mousey);

				int size_x;
				int size_y;
				glfwGetWindowSize(parentAppData->window, &size_x, &size_y);

				glfwSetCursorPos(parentAppData->window, size_x / 2, size_y / 2);

				if (!was_focused)
				{
					lastX = mousex;
					lastY = mousey;
					firstFrame = false;
				}

				double xoffset = mousex - (size_x / 2);
				double yoffset = (size_y / 2) - mousey;

				lastX = mousex;
				lastY = mousey;

				parentAppData->renderer.camera.ProcessMouseMovement(xoffset, yoffset);
				
				

			}

			void ProcessInputs() override
			{
				if (glfwGetKey(parentAppData->window, GLFW_KEY_W) == GLFW_PRESS)
					parentAppData->renderer.camera.ProcessKeyboard(FORWARD, parentAppData->deltaTime);

				if (glfwGetKey(parentAppData->window, GLFW_KEY_S) == GLFW_PRESS)
					parentAppData->renderer.camera.ProcessKeyboard(BACKWARD, parentAppData->deltaTime);

				if (glfwGetKey(parentAppData->window, GLFW_KEY_A) == GLFW_PRESS)
					parentAppData->renderer.camera.ProcessKeyboard(LEFT, parentAppData->deltaTime);

				if (glfwGetKey(parentAppData->window, GLFW_KEY_D) == GLFW_PRESS)
					parentAppData->renderer.camera.ProcessKeyboard(RIGHT, parentAppData->deltaTime);

				if (glfwGetKey(parentAppData->window, GLFW_KEY_SPACE) == GLFW_PRESS)
					parentAppData->renderer.camera.ProcessKeyboard(UP, parentAppData->deltaTime);

				if (glfwGetKey(parentAppData->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
					parentAppData->renderer.camera.ProcessKeyboard(DOWN, parentAppData->deltaTime);

				if (glfwGetKey(parentAppData->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				{
					is_focused = false;
					ImGui::SetWindowFocus(NULL);
				}

				if (is_focused)
				{
					ProcessMouse();
				}
			}

			private:
				ImVec2 viewportCenter;

		};

	}
}