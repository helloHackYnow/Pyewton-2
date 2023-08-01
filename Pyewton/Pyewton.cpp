#include "Pyewton.h"
#include <glad/glad.h>
#include <imgui.h>
#include "imgui_stdlib.h"

namespace fs = std::filesystem;

Pyewton::Pyewton()
{
	
}

void Pyewton::Init(GLFWwindow* window_, const char* glsl_version) {

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

	//Init renderer
	InitFbo();

	//setup Renderer
	fs::path current_dir = fs::current_path();
	fs::path vertex_path("shader_dir/vertex.glsl");
	fs::path fragment_path("shader_dir/fragment.glsl");
	fs::path full_v = current_dir / vertex_path;
	fs::path full_f = current_dir / fragment_path;
	renderer.InitRenderer(full_v.string().data(), full_f.string().data(), 800, 600);

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



void Pyewton::NewFrame() {
	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}


void Pyewton::UpdateUI() {
	
	ImGui::DockSpaceOverViewport();
	
	DrawBodyList(),

	DrawViewport();

	DrawSimulationSettings();
}

void Pyewton::UpdateInputs()
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

void Pyewton::UpdateTiming()
{
	currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Pyewton::RenderUI() {
	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Pyewton::RenderScene()
{
	glViewport(0, 0, render_width, render_height);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	renderer.Render(bodyList, fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Pyewton::Shutdown() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Pyewton::DrawViewport()
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
	rescaleFbo();
	ImGui::Image((ImTextureID)texID, wsize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::EndChild();
	ImGui::End();
}

void Pyewton::DrawBodyList()
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

void Pyewton::DrawSimulationSettings()
{
	ImGui::Begin("Simulation settings");

	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(300, 0), true, window_flags);
		
		ImGui::SeparatorText("Appearance");
		ImGui::ColorEdit3("Ambient", renderer.ambientLight.getColor());

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

	ImGui::End();
}

void Pyewton::InitFbo()
{
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width, render_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

	glGenRenderbuffers(1, &rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, render_width, render_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Pyewton::rescaleFbo()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width, render_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, render_width, render_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboID);

	
}

