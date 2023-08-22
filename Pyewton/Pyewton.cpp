#include "Pyewton.h"
#include <glad/glad.h>
#include <imgui.h>
#include "imgui_stdlib.h"
#include "Files.h"

#define MULTITHREADING 0

namespace fs = std::filesystem;

namespace Pyewton
{


	Application::Application()
	{

	}

	void Application::Init(GLFWwindow* window_, const char* glsl_version) {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO* io = &ImGui::GetIO();
		io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window_, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::StyleColorsDark();

		glfwGetWindowSize(window_, &render_width, &render_height);

		renderer.InitRenderer(800, 600);

		renderer.mainShader = renderer.AddShader(GetShaderPath("vertex.glsl").string().data(), GetShaderPath("fragment.glsl").string().data());

		renderer.SetShaderFlag(renderer.mainShader, shaderFlags_NeedAmbientLight);
		renderer.SetShaderFlag(renderer.mainShader, shaderFlags_NeedLightPoints);
		renderer.SetShaderFlag(renderer.mainShader, shaderFlags_NeedCameraInfo);

		renderer.orbitShader = renderer.AddShader(GetShaderPath("orbit_v.glsl").string().data(), GetShaderPath("orbit_f.glsl").string().data());
		renderer.SetShaderFlag(renderer.orbitShader, shaderFlags_NeedCameraInfo);

		//setup body list
		Body sphere1;
		Body sphere2;
		Body sphere3;

		sphere1.velocity = glm::vec3(0, 0, 10);
		sphere1.setPos(-10.f, 0.f, 0.f);


		sphere2.setColor(1, 1, 1);
		sphere2.setPos(10.f, 0.f, 0.f);
		sphere2.velocity = glm::vec3(0, 0, -10);
		sphere2.name = std::string("hello");
		sphere2.setLight(Color(1), 4);
		sphere2.isEmissive = false;

		sphere3.affectOther = false;
		sphere3.isAffected = false;
		sphere3.isEmissive = true;
		sphere3.setLight(Color(1, 1, 1), 5);
		sphere3.name = std::string("Light");
		 
		bodyList.push_back(sphere1);
		bodyList.push_back(sphere2);
		bodyList.push_back(sphere3);

		//ImGui styling
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 6.f;

		//Init time
		currentFrame = static_cast<float>(glfwGetTime());
		lastFrame = currentFrame;
		deltaTime = 0;

		//window pointer 
		window = window_;

#ifdef MULTITHREADING
		simulation.SetModel(Frigg::Simulate);
		//simulation.Start(&bodyList, 1.f/60.f);

#endif // MULTITHREADING

		//Fonts
		ImFontConfig fontCfg;
		fontCfg.FontDataOwnedByAtlas = false;
		io->Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &fontCfg);

		// Init notifications
		ImGui::MergeIconsWithLatestFont(16.f, false);

	}

	void Application::MainLoop()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


#ifndef MULTITHREADING
		if(isSimulationRunning) Frigg::Simulate(&bodyList, 1.f / 60.f);
#endif // !MULTITHREADING


		UpdateInputs();
		UpdateTiming();

		NewFrame();
		UpdateUI();
		
		UpdateReplay();
		RenderScene();

		RenderUI();
	}



	void Application::NewFrame() {
		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}


	void Application::UpdateUI() {

		ImGui::DockSpaceOverViewport();

		DrawBodyList(),

		DrawViewport();

		DrawSimulationSettings();

		DrawSimulationControl();

		DrawCameraParameters();

		DrawMultithreadingDebug();

		DrawPrecomputeDebug();

		DrawPrecomputePlayer();

		DrawNotifications();
	}

	void Application::UpdateInputs()
	{
		//Mouse input
		static double lastX = 0;
		static double lastY = 0;
		static bool firstFrame = true;

		if (isUsingCamera)
		{
			double mousex;
			double mousey;

			glfwGetCursorPos(window, &mousex, &mousey);

			if (firstFrame)
			{
				lastX = mousex;
				lastY = mousey;
				firstFrame = false;
			}

			double xoffset = mousex - lastX;
			double yoffset = lastY - mousey;

			lastX = mousex;
			lastY = mousey;

			renderer.camera.ProcessMouseMovement(xoffset, yoffset);
		}
		else
		{
			firstFrame = true;
		}


		//Keyboard input
		static float lastTABpress = 0;
		const float tabCooldown = 0.5f;
		
		/*
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		*/

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			renderer.camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			renderer.camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			renderer.camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			renderer.camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			renderer.camera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			renderer.camera.ProcessKeyboard(DOWN, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && (currentFrame - lastTABpress) > tabCooldown)
		{

			isUsingCamera = !isUsingCamera;
			lastTABpress = currentFrame;

			if (isUsingCamera)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

	void Application::UpdateTiming()
	{
		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void Application::UpdateReplay()
	{
		if (isReplaying)
		{
			player.Next();
		}
	}

	void Application::RenderUI() {

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
	}

	void Application::RenderScene()
	{
		if (isReplaying) renderer.Render(player.displayBodyList);
		else renderer.Render(bodyList, &bodyList_access);
	}

	void Application::Shutdown() {
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::DrawViewport()
	{
		ImGui::Begin("Viewport");

		ImGui::BeginChild("GameRender");

		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();
		ImVec2 wpos = ImGui::GetWindowPos();

		render_width = wsize.x;
		render_height = wsize.y;
		//reste viewport for the renderer
		renderer.setViewport(render_width, render_height);
		ImGui::Image((ImTextureID)renderer.GetTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::EndChild();
		ImGui::End();
	}

	void Application::DrawBodyList()
	{
		ImGui::Begin("Body list");
		int i = 0;
		for (auto& body : bodyList)
		{
			std::string node_name = std::to_string(i) + " - " + body.name;
			if (ImGui::TreeNode(node_name.c_str()))
			{
				ImGui::DragFloat3("position", &body.position[0]);

				ImGui::SeparatorText("orbit");
				ImGui::Text(std::string(std::to_string(body.orbit.vertexList.size())).data());

				ImGui::SeparatorText("Material");
				ImGui::Checkbox("isEmissive", &body.isEmissive);
				ImGui::BeginDisabled(!body.isEmissive);
				ImGui::DragFloat("Strength", &body.light.strength, 0.1f);
				ImGui::ColorEdit3("Light color", body.light.color.getColor());
				ImGui::EndDisabled();

				ImGui::SeparatorText("Simulation");
				ImGui::DragFloat("Mass", &body.mass, 0.1f, 0.f, 10000.f);
				ImGui::Checkbox("Is affected", &body.isAffected);
				ImGui::Checkbox("Affect other", &body.affectOther);

				ImGui::SeparatorText("Appearance");
				ImGui::ColorEdit3("Color", body.color.getColor());
				ImGui::SliderFloat("Size", &body.size, 0.01f, 10.f);

				ImGui::TreePop();
			}
			i++;
		}
		if (ImGui::Button("Add planet"))
		{
			bodyList.push_back(Body("hello"));
		}
		ImGui::SameLine();
		ImGui::Button("delete");
		ImGui::End();
	}

	void Application::DrawSimulationControl()
	{
		if (ImGui::Begin("Simulation Control"))
		{
			ImGui::Checkbox("isSimulationRunning", &isSimulationRunning);
		}

		if (ImGui::Button("Reset Orbits"))
		{
			bodyList_access.lock();
			for (auto& body : bodyList)
			{
				body.ResetOrbit();
			}
			bodyList_access.unlock();
		}
		ImGui::End();
	}

	void Application::DrawSimulationSettings()
	{
		ImGui::Begin("Simulation settings");

		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("ChildR", ImVec2(300, 0), true, window_flags);

			ImGui::SeparatorText("Appearance");
			ImGui::ColorEdit3("Ambient", renderer.ambientLight.getColor());

			ImGui::EndChild();
			ImGui::SameLine();
			
			ImGui::Checkbox("Postprocessing", &renderer.postprocesingEnable);

			ImGui::PopStyleVar();
		}

		ImGui::End();
	}

	void Application::DrawCameraParameters()
	{
		if (ImGui::Begin("Camera"))
		{
			ImGui::DragFloat("Speed", &renderer.camera.MovementSpeed, 0.1);
			ImGui::DragFloat("FOV", &renderer.camera.Zoom, 0.1f, 1, 120);
		}
		ImGui::End();
	}

	void Application::DrawMultithreadingDebug()
	{
		static auto begining = std::chrono::steady_clock::now();
		static int lastPassnb = 0;
		static std::vector<float> nbPass;
		static int pushControl = 0;

		auto current = std::chrono::steady_clock::now();
		auto sinceFirst = std::chrono::duration_cast<std::chrono::seconds>(current - begining);

		simulation.infos.mutex.lock();

		pushControl = (pushControl + 1) % 60;
		if (pushControl == 0)
		{

			nbPass.push_back(simulation.infos.nbIterations);
		}

		//float passPerSec_average = simulation.infos.nbIterations / sinceFirst.count();
		//passPerSec_average /= pow(10, 9); // Convert from nanoseconds to seconds

		float passPerSec_average = simulation.infos.nbIterations / (sinceFirst.count() + 1);
		//passPerSec_current /= pow(10, 9); // Convert from nanoseconds to seconds

		lastPassnb = simulation.infos.nbIterations;
		if (ImGui::Begin("Multithreading"))
		{
			ImGui::BeginDisabled(simulation.infos.isSimulationRunning);
			if (ImGui::Button("start sim"))
			{
				simulation.Start(&bodyList, 1.f/60.f);
			}
			ImGui::EndDisabled();
			ImGui::SameLine();
			ImGui::BeginDisabled(!simulation.infos.isSimulationRunning);
			if (ImGui::Button("stop sim"))
			{
				simulation.Stop();
			}
			ImGui::EndDisabled();

			ImGui::DragInt("SimPerBatch", &simulation.infos.simulationPerBatch);
			ImGui::DragInt("ns between batchs", (int*)&simulation.infos.durationBtwBatch, 1.f, 250, 25000);
			ImGui::Text("pass per sec : %.0f", passPerSec_average);
			ImGui::PlotLines("", nbPass.data(), nbPass.size(), 0, (const char*)0, FLT_MAX, FLT_MAX, ImVec2(0, 200));
			ImGui::End();
		}
		simulation.infos.mutex.unlock();
		
	}

	void Application::DrawPrecomputeDebug()
	{
		


		if (ImGui::Begin("Precompute"))
		{
			if (ImGui::Button("Precompute - start"))
			{
				auto holder_ = simulation.Precompute(bodyList, 1.0f / 60.f, 10000);
				holders.push_back(std::move(holder_));
				ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Hello World! This is a success! %s", "We can also format here:)" });
			}

			int i = 0;
			for (auto& holder : holders)
			{
				if (ImGui::TreeNode(std::to_string(i).data()))
				{
					ImGui::Text("Size : %d", holder.get()->size);
					ImGui::Text("Simulated : %d", holder.get()->back_state);

					if (ImGui::Button("Load in player"))
					{
						player.SetHolder(holder.get());
						player.SetBodyList(bodyList);
						isHolderLoaded = true;
						loadedHolderIndex = i;
					}

					ImGui::BeginDisabled(isReplaying);
					if (ImGui::Button("Delete"))
					{
						holders.erase(holders.begin() + i);
					}
					ImGui::EndDisabled();

					ImGui::TreePop();
				}

				i++;
			}

			ImGui::End();
		}
	}

	void Application::DrawPrecomputePlayer()
	{
		char buf[128];
		const char* title_{ isHolderLoaded ? "Player - %d###Player" : "Player###Player" };
		sprintf(buf, title_, loadedHolderIndex);

		if (ImGui::Begin(buf))
		{
			if (ImGui::ArrowButton("#play", ImGuiDir_Right))
			{
				isReplaying = !isReplaying;
				player.ResetReplayPos();
			}
			ImGui::SameLine();

			if (isReplaying) ImGui::Text("Playing ...");
			else ImGui::Text("Stoped");

			if (isReplaying)
			{
				ImGui::Text("%d", player.replayPos); 
			}

			ImGui::DragFloat("Speed", &player.replaySpeed, 0.1f, 1.f, 10.f);

			ImGui::End();
		}
	}

	void Application::DrawNotifications()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
		ImGui::RenderNotifications(); // <-- Here we render all notifications
		ImGui::PopStyleVar(1); // Don't forget to Pop()
		ImGui::PopStyleColor(1);
	}




}