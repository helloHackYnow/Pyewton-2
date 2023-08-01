#include "Renderer.h"




Renderer::Renderer()
{

}

void Renderer::InitRenderer(const char* vertex_path, const char* fragment_path, float width, float height)
{
	Renderer::setViewport(width, height);

	mainShader = Shader(vertex_path, fragment_path);
}

void Renderer::Render(std::vector<Body>& bodyList, unsigned int fbo)
{
	//Create a list of all emissive bodies
	std::vector<PointLight> lightList;
	bool isAnyLight = false;
	for (auto body : bodyList)
	{
		if (body.isEmissive)
		{
			lightList.push_back(body.light);
			lightList.back().position = body.position;
			isAnyLight = true;
		}
	}

	glClearColor(ambientLight.getColor()[0]/2, ambientLight.getColor()[1]/2, ambientLight.getColor()[2]/2, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainShader.use();

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), viewport_width / viewport_height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	mainShader.setMat4("projection", projection);
	mainShader.setMat4("view", view);

	// Light
	mainShader.setVec3("ambient", ambientLight);

	mainShader.setInt("nb_lights", lightList.size());
	if (isAnyLight)
	{
		char var_pos[] = "light_points[0].position";
		char var_col[] = "light_points[0].color";
		for (int i = 0; i < lightList.size(); i++)
		{
			var_pos[13] = '0' + i;
			var_col[13] = '0' + i;
			mainShader.setVec3(var_pos, lightList.at(i).position);
			mainShader.setVec3(var_col, lightList.at(i).color);
		}
		
	}
	else
	{
		//Black light so the shader don't show the old light
		mainShader.setVec3("lightColor", 0, 0 ,0);
	}

	for (auto& body : bodyList)
	{
		body.Draw(mainShader);
	}
}


void Renderer::setViewport(float width, float height)
{
	viewport_height = height;
	viewport_width = width;
}

