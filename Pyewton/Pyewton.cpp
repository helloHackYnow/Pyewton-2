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

	void Application::SetupImGuiStyle()
	{
		// Future Dark style by rewrking from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(12.0f, 12.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(20.0f, 20.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(6.0f, 6.0f);
		style.FrameRounding = 0.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(12.0f, 6.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
		style.CellPadding = ImVec2(12.0f, 6.0f);
		style.IndentSpacing = 20.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabMinSize = 12.0f;
		style.GrabRounding = 0.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
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
		SetupImGuiStyle();

		InitSubApps();

		controllers.simulationController = new Frigg::SimulationController(&data.holders);

	}

	

	void Application::InitSubApps()
	{
		
		AddSubApps<App::Viewport>("viewport");
		AddSubApps<App::BodyList>("bodylist");
		
	}

	template<typename SubApp_Type> void Application::AddSubApps(std::string name)
	{
		SubApp_Type sub_app;
		sub_app.Init(&data);
		sub_app.name = name;

		subApps.push_back(std::make_unique<SubApp_Type>(sub_app));
	}

	void Application::MainLoop()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		UpdateInputs();
		UpdateTiming();

		NewFrame();
		UpdateUI();
		UpdateFileDialog();
		
		UpdateReplay();
		RenderScene();

		RenderUI();

		UpdateFocusedSubAppIndex();
	}



	void Application::NewFrame() {
		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}


	void Application::UpdateUI() {

		ImGui::DockSpaceOverViewport();


		//DrawBodyList();

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
		if (data.focused_subApp_index != -1)
		{
			subApps.at(data.focused_subApp_index)->ProcessInputs();
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

	void Application::UpdateFocusedSubAppIndex()
	{
		if (data.focused_subApp_index != -1) std::cout << "focused app : " << subApps.at(data.focused_subApp_index)->name << " \n";
		else std::cout << "None focused\n";
		int i = 0;
		for (auto & subApp : subApps)
		{
			if (subApp->is_focused)
			{
				data.focused_subApp_index = i;
				return;
			}
			i++;
		}
		data.focused_subApp_index = -1;

	}

	void Application::RenderUI() {

		// Render dear imgui into screen
		ImGui::Render();
	}

	void Application::RenderScene()
	{
		if (data.isReplaying) data.renderer.Render(data.player.displayBodyList);
		else data.renderer.Render(data.editableSystem);
	}

	void Application::Shutdown() {
		delete controllers.simulationController;

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::DrawSubApps()
	{ 
		bool is_any_focused = false;
		for (auto & app : subApps)
		{
			if (app->is_open)
			{
				app->Draw();
			}

			if (app->is_focused)
			{
				is_any_focused = true;
			}
		}

		if (!is_any_focused)
		{
			//ImGui::SetWindowFocus(NULL);
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
			ImGui::Checkbox("Draw Grid", &data.renderer.drawGridEnable);

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
		static int nb_iterations = 1000000;
		if (ImGui::Begin("Precompute"))
		{
			ImGui::DragInt("Nb iterations", &nb_iterations, 100);
			// Calcul de la taille du holder
			unsigned long int holder_size = sizeof(glm::vec3) * data.editableSystem.size() *( nb_iterations / (1024.0*1024.0));
			ImGui::Text("Size in memory : %i Mo", holder_size);
			

			if (ImGui::Button("Precompute - start"))
			{
				Frigg::SimulationRequest request;
				request.iterations = nb_iterations;
				request.systemInitialState = data.editableSystem;
				request.time_btwn_iterations = 1 / 60.f;

				controllers.simulationController->AddRequestToQueue(request);
			}

			int i = 0;
			for (auto& holder : data.holders)
			{
				ImGui::BeginDisabled(holder.get()->isPrecomputing);
				bool is_tree_open = ImGui::TreeNode(std::to_string(i).data());

				if (holder.get()->isPrecomputing)
				{
					ImGui::SameLine();
					ImSpinner::SpinnerDotsLoading("Spinner", 7, 2, ImColor(1.f, 1.f, 1.f), ImColor(0, 0, 0), 1);
				}

				if (is_tree_open)
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
				if (ImGui::MenuItem("Save", nullptr, false, true))
				{
					SaveFunction();
				}

				if (ImGui::MenuItem("Open", nullptr, false, true))
				{
					LoadFunction();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				for (auto& subApp : subApps)
				{
					ImGui::MenuItem(subApp->name.data(), nullptr, &subApp->is_open);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

	}

	void Application::SaveFunction()
	{

		ifd::FileDialog::Instance().Save("JsonSaveDialog", "Save into a json", "JSON file (*.json){.json},.*", GetJsonFolder().string());

		//ifd::FileDialog::Instance().IsDone("ShaderOpenDialog");
		data.is_saving = true;

	}

	void Application::LoadFunction()
	{
		ifd::FileDialog::Instance().Open("JsonOpenDialog", "Open a json file", "JSON file (*.json){.json},.*", false, GetJsonFolder().string());

		data.is_opening = true;
	}

	void Application::SyncBodyListsStyle()
	{
		int i = 0;

		for (auto& body : data.editableSystem)
		{
			data.displaySystem.at(i).appearance = body.appearance;
			i++;
		}
	}
	void Application::UpdateFileDialog()
	{
		if (data.is_saving)
		{
			if (ifd::FileDialog::Instance().IsDone("JsonSaveDialog")) {
				if (ifd::FileDialog::Instance().HasResult()) {
					std::string res = ifd::FileDialog::Instance().GetResult().u8string();
					Json::SaveToFile(data.editableSystem, res.data());
				}
				ifd::FileDialog::Instance().Close();
				data.is_saving = false;
			}
		}

		if (data.is_opening)
		{
			if (ifd::FileDialog::Instance().IsDone("JsonOpenDialog")) {
				if (ifd::FileDialog::Instance().HasResult()) {
					std::string res = ifd::FileDialog::Instance().GetResult().u8string();
					data.editableSystem = Json::LoadFile(res.data());
				}
				ifd::FileDialog::Instance().Close();
				data.is_opening = false;
			}
		}
	}
}