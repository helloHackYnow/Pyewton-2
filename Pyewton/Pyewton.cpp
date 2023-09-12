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

		//Style
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.f);

		glfwGetWindowSize(window_, &data.render_width, &data.render_height);

		data.renderer.InitRenderer(800, 600);

		data.renderer.mainShader = data.renderer.AddShader(GetShaderPath("vertex.glsl").string().data(), GetShaderPath("fragment.glsl").string().data());

		data.renderer.SetShaderFlag(data.renderer.mainShader, shaderFlags_NeedAmbientLight);
		data.renderer.SetShaderFlag(data.renderer.mainShader, shaderFlags_NeedLightPoints);
		data.renderer.SetShaderFlag(data.renderer.mainShader, shaderFlags_NeedCameraInfo);
		data.renderer.SetShaderFlag(data.renderer.mainShader, shaderFlags_NeedExposure);

		data.renderer.orbitShader = data.renderer.AddShader(GetShaderPath("orbit.vert").string().data(), GetShaderPath("orbit.frag").string().data());
		data.renderer.SetShaderFlag(data.renderer.orbitShader, shaderFlags_NeedCameraInfo);

		data.renderer.gridShader = data.renderer.AddShader(GetShaderPath("grid.vert").string().data(), GetShaderPath("grid.frag").string().data());
		data.renderer.SetShaderFlag(data.renderer.gridShader, shaderFlags_NeedCameraInfo);

		//setup body list
		Body sphere1;
		Body sphere2;
		Body sphere3;

		sphere1.physics.velocity = glm::vec3(0, 0, 10);
		sphere1.setPos(-10.f, 0.f, 0.f);


		sphere2.setColor(1, 1, 1);
		sphere2.setPos(10.f, 0.f, 0.f);
		sphere2.physics.velocity = glm::vec3(0, 0, -10);
		sphere2.name = std::string("hello");
		sphere2.setLight(Color(1), 4);
		sphere2.appearance.isEmissive = false;

		sphere3.physics.affectOther = false;
		sphere3.physics.isAffected = false;
		sphere3.appearance.isEmissive = true;
		sphere3.setLight(Color(1, 1, 1), 5);
		sphere3.name = std::string("Light");
		 
		data.editableSystem.push_back(sphere1);
		data.editableSystem.push_back(sphere2);
		data.editableSystem.push_back(sphere3);

		//ImGui styling
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 6.f;

		//Init time
		data.currentFrame = static_cast<float>(glfwGetTime());
		data.lastFrame = data.currentFrame;
		data.deltaTime = 0;

		//window pointer 
		data.window = window_;

#ifdef MULTITHREADING
		data.simulation.SetModel(Frigg::Simulate);
		//simulation.Start(&bodyList, 1.f/60.f);

#endif // MULTITHREADING

		//Fonts
		ImFontConfig fontCfg;
		fontCfg.FontDataOwnedByAtlas = false;
		io->Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &fontCfg);

		// Init notifications
		ImGui::MergeIconsWithLatestFont(16.f, false);

	}

	void Application::InitSubApps()
	{
		Viewport viewport;
		viewport.Init(&data);

		subApps.push_back(viewport);
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


		DrawBodyList();

		DrawSubApps();

		/*
		if (show_viewport)
		{
			DrawViewport();

		}
		*/

		DrawSimulationSettings();

		if (show_cameraParameters)
		{
			DrawCameraParameters();
		}

		DrawPrecomputeDebug();

		DrawPrecomputePlayer();

		DrawNotifications();

		DrawMenu();
	}

	void Application::UpdateInputs()
	{
		//Mouse input
		static double lastX = 0;
		static double lastY = 0;
		static bool firstFrame = true;

		if (data.isUsingCamera)
		{
			double mousex;
			double mousey;

			glfwGetCursorPos(data.window, &mousex, &mousey);

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

			data.renderer.camera.ProcessMouseMovement(xoffset, yoffset);
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

		if (glfwGetKey(data.window, GLFW_KEY_W) == GLFW_PRESS)
			data.renderer.camera.ProcessKeyboard(FORWARD, data.deltaTime);
		if (glfwGetKey(data.window, GLFW_KEY_S) == GLFW_PRESS)
			data.renderer.camera.ProcessKeyboard(BACKWARD, data.deltaTime);
		if (glfwGetKey(data.window, GLFW_KEY_A) == GLFW_PRESS)
			data.renderer.camera.ProcessKeyboard(LEFT, data.deltaTime);
		if (glfwGetKey(data.window, GLFW_KEY_D) == GLFW_PRESS)
			data.renderer.camera.ProcessKeyboard(RIGHT, data.deltaTime);
		if (glfwGetKey(data.window, GLFW_KEY_SPACE) == GLFW_PRESS)
			data.renderer.camera.ProcessKeyboard(UP, data.deltaTime);
		if (glfwGetKey(data.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			data.renderer.camera.ProcessKeyboard(DOWN, data.deltaTime);

		if (glfwGetKey(data.window, GLFW_KEY_TAB) == GLFW_PRESS && (data.currentFrame - lastTABpress) > tabCooldown)
		{

			data.isUsingCamera = !data.isUsingCamera;
			lastTABpress = data.currentFrame;

			if (data.isUsingCamera)
			{
				glfwSetInputMode(data.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			else
			{
				glfwSetInputMode(data.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

	void Application::UpdateTiming()
	{
		data.currentFrame = static_cast<float>(glfwGetTime());
		data.deltaTime = data.currentFrame - data.lastFrame;
		data.lastFrame = data.currentFrame;
	}

	void Application::UpdateReplay()
	{
		if (data.isReplaying)
		{
			data.player.Next();
		}
	}

	void Application::RenderUI() {

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
	}

	void Application::RenderScene()
	{
		if (data.isReplaying) data.renderer.Render(data.player.displayBodyList);
		else data.renderer.Render(data.editableSystem);
	}

	void Application::Shutdown() {
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::DrawSubApps()
	{
		for (auto & app : subApps)
		{
			if (app.is_open)
			{
				app.Draw();
			}
		}
	}

	void Application::DrawViewport()
	{
		ImGuiWindowFlags window_flag = 0;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		if (!ImGui::Begin("Dear ImGui Demo", &show_viewport, window_flag))
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

			data.render_width = wsize.x;
			data.render_height = wsize.y;
			//reste viewport for the renderer
			data.renderer.setViewport(data.render_width, data.render_height);
			ImGui::Image((ImTextureID)data.renderer.GetTexture(), wsize, ImVec2(0, 1), ImVec2(1, 0));

			ImGui::EndChild();
			ImGui::End();
		}

		ImGui::PopStyleVar();
	}

	void Application::DrawBodyList()
	{
		ImGui::Begin("Body list");
		int i = 0;
		for (auto& body : data.editableSystem)
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
			data.editableSystem.push_back(Body("hello"));
		}
		ImGui::SameLine();
		ImGui::Button("delete");
		ImGui::End();
	}

	void Application::DrawSimulationSettings()
	{

		ImGui::Begin("Simulation settings", &show_cameraParameters);

		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("ChildR", ImVec2(300, 0), true, window_flags);

			ImGui::SeparatorText("Appearance");
			ImGui::ColorEdit3("Ambient", data.renderer.ambientLight.getColor());
			ImGui::DragFloat("Exposure", &data.renderer.exposure, 0.1f, 0.f);

			ImGui::EndChild();
			ImGui::SameLine();
			
			ImGui::Checkbox("Postprocessing", &data.renderer.postprocesingEnable);

			ImGui::PopStyleVar();
		}

		ImGui::End();
	}

	void Application::DrawCameraParameters()
	{
		if (ImGui::Begin("Camera"))
		{
			ImGui::DragFloat("Speed", &data.renderer.camera.MovementSpeed, 0.1);
			ImGui::DragFloat("FOV", &data.renderer.camera.Zoom, 0.1f, 1, 120);
		}
		ImGui::End();
	}

	void Application::DrawPrecomputeDebug()
	{
		if (ImGui::Begin("Precompute"))
		{
			if (ImGui::Button("Precompute - start"))
			{
				auto holder_ = data.simulation.Precompute(data.editableSystem, 1.0f / 60.f, 500000,
					[]() {
						ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Finish !" });
					}
				);

				data.holders.push_back(std::unique_ptr<Frigg::SimulationHolder>(holder_));
				//ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Hello World! This is a success! %s", "We can also format here:)" });
			}

			int i = 0;
			for (auto& holder : data.holders)
			{
				ImGui::BeginDisabled(holder.get()->isPrecomputing);
				if (ImGui::TreeNode(std::to_string(i).data()))
				{
					if (ImGui::Button("Load in player"))
					{
						data.displaySystem = data.editableSystem;

						data.player.SetHolder(holder.get());
						data.player.SetBodyList(data.displaySystem);
						data.isHolderLoaded = true;
						data.loadedHolderIndex = i;
					}

					ImGui::BeginDisabled(data.isReplaying);
					if (ImGui::Button("Delete"))
					{
						data.holders.erase(data.holders.begin() + i);
					}
					ImGui::EndDisabled();

					ImGui::TreePop();
				}
				ImGui::EndDisabled();

				i++;
			}

			ImGui::End();
		}
	}

	void Application::DrawPrecomputePlayer()
	{
		char buf[128];
		const char* title_{ data.isHolderLoaded ? "Player - %d###Player" : "Player###Player" };
		sprintf(buf, title_, data.loadedHolderIndex);

		if (ImGui::Begin(buf))
		{
			ImGui::BeginDisabled(!data.isHolderLoaded);
			if (ImGui::ArrowButton("#play", ImGuiDir_Right))
			{
				data.isReplaying = !data.isReplaying;
				data.player.ResetReplayPos();
			}
			ImGui::SameLine();

			if (data.isReplaying) ImGui::Text("Playing ...");
			else ImGui::Text("Stoped");

			if (data.isReplaying)
			{
				ImGui::Text("%f", data.player.replayPos); 
			}

			ImGui::DragFloat("Speed", &data.player.replaySpeed, 0.1f, 0.1f, 1000.f);

			ImGui::EndDisabled();
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

	void Application::DrawMenu()
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Viewport", nullptr, &show_viewport);
				ImGui::MenuItem("Camera", nullptr, &show_cameraParameters);
				ImGui::MenuItem("Precompute", nullptr, &show_precomputeDebug);

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

	}

	void Application::SyncBodyLists()
	{
		int i = 0;

		for (auto& body : data.editableSystem)
		{

		}
	}
}