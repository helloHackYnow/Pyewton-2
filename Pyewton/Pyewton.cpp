#include "Pyewton.h"
#include <glad/glad.h>
#include <imgui.h>
#include "imgui_stdlib.h"
#include "Files.h"

namespace fs = std::filesystem;

namespace Pyewton
{


	Application::Application()
	{

	}

	void Application::Init(GLFWwindow* window_, const char* glsl_version) {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


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
	}

	void Application::MainLoop()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if(isSimulationRunning) Simulate(bodyList, 1.f / 60.f);

		UpdateInputs();
		UpdateTiming();

		NewFrame();
		UpdateUI();

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

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

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

	void Application::RenderUI() {
		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Application::RenderScene()
	{
		renderer.Render(bodyList);
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

				ImGui::SeparatorText("Material");
				ImGui::Checkbox("isEmissive", &body.isEmissive);
				ImGui::BeginDisabled(!body.isEmissive);
				ImGui::DragFloat("Strength", &body.light.strength, 0.1f);
				ImGui::ColorEdit3("Light color", body.light.color.getColor());
				ImGui::EndDisabled();

				ImGui::SeparatorText("Simulation");
				ImGui::DragFloat("Mass", &body.mass, 0.1f, 0.f, 100.f);
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
			for (auto& body : bodyList)
			{
				body.ResetOrbit();
			}
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
			ImGui::BeginChild("ChildR2", ImVec2(300, 0), true, window_flags);

			ImGui::SeparatorText("Bloom");
			ImGui::DragFloat("knee", &renderer.bloom.knee, 0.05f);
			ImGui::DragFloat("threshold", &renderer.bloom.threshold, 0.05f);
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}

		ImGui::End();
	}



}