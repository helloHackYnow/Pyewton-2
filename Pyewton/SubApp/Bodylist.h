#pragma once

#include "SubApp.h"

namespace Pyewton::App
{
	class BodyList : public SubApp
	{
	public:
		using SubApp::SubApp;
		void Draw() override
		{
			if (!ImGui::Begin(name.data(), &is_open))
			{
				ImGui::End();
			}
			else
			{
				//Update focus
				is_focused = ImGui::IsWindowFocused();

				int i = 0;
				for (auto& body : parentAppData->editableSystem)
				{
					std::string node_name = std::to_string(i) + " - " + body.name;
					if (ImGui::TreeNode(node_name.c_str()))
					{
						ImGui::DragFloat3("position", &body.physics.position[0]);

						ImGui::SeparatorText("orbit");
						ImGui::Text(std::string(std::to_string(body.orbit.vertexList.size())).data());

						ImGui::SeparatorText("Material");
						ImGui::Checkbox("isEmissive", &body.appearance.isEmissive);
						ImGui::BeginDisabled(!body.appearance.isEmissive);
						ImGui::DragFloat("Strength", &body.appearance.light.strength, 0.1f);
						ImGui::ColorEdit3("Light color", body.appearance.light.color.getColor());
						ImGui::EndDisabled();

						ImGui::SeparatorText("Simulation");
						ImGui::DragFloat("Mass", &body.physics.mass, 0.1f, 0.f, 10000.f);
						ImGui::Checkbox("Is affected", &body.physics.isAffected);
						ImGui::Checkbox("Affect other", &body.physics.affectOther);

						ImGui::SeparatorText("Appearance");
						ImGui::ColorEdit3("Color", body.appearance.color.getColor());
						ImGui::SliderFloat("Size", &body.appearance.size, 0.01f, 10.f);

						ImGui::TreePop();
					}
					i++;
				}
				if (ImGui::Button("Add planet"))
				{
					parentAppData->editableSystem.push_back(Body("hello"));
				}
				ImGui::SameLine();
				ImGui::Button("delete");
				ImGui::End();
			}
		}


	};
}