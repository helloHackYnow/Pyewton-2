#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Pyewton.h"
#include "Renderer/test.h"
#include "Body/Body.h"
#include <filesystem>
#include <string>
#include "Framebuffer.h"
#include <imgui_impl_glfw.h>
#include "Simulation/Simulation.h"

namespace fs = std::filesystem;
  
//File path infos
fs::path current_dir;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
unsigned int SCR_WIDTH = 1200;
unsigned int SCR_HEIGHT = 800;

int main()
{
	current_dir = fs::current_path();

	// Setup window
	if (!glfwInit())
		return 1;

	// GL 4.5 + GLSL 450
	const char* glsl_version = "#version 450";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pyewton Sandbox", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // tie window context to glad's opengl funcs
		throw("Unable to context to OpenGL");
	
	glEnable(GL_DEPTH_TEST);

	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Pyewton app;
	app.Init(window, glsl_version);


	while (!glfwWindowShouldClose(window)) {

		Simulate(&(app.bodyList), 1.f / 60.f);

		glfwPollEvents();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		app.UpdateInputs();
		app.UpdateTiming(); 

		app.NewFrame();
		app.UpdateUI(); 
		
		app.RenderScene();

		app.RenderUI();
		 
		
		ImGuiIO& io = ImGui::GetIO();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		
		glfwSwapBuffers(window);
	}
	app.Shutdown();
	 
	return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	
}

