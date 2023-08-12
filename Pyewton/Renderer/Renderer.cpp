#include "Renderer.h"
 


namespace fs = std::filesystem;

namespace Pyewton::Odin
{
	Renderer::Renderer()
	{

	}

	void Renderer::InitRenderer(int width, int height)
	{
		Renderer::setViewport(width, height);
		fbo_internal.Init(width, height, true, 1);
		fbo_postProcess.Init(width, height, true, 1);
		fbo_output.Init(width, height, true, 1);


		//Init postProcessor
		postProcessor.Init();
		bloom.Init();

	}

	int Renderer::AddShader(const char* vertex_path, const char* fragment_path)
	{
		Shader* shader = new GShader(vertex_path, fragment_path);

		std::unique_ptr<Shader> shader_ptr(shader);

		shaderList.push_back(std::move(shader_ptr));

		return shaderList.size() - 1;
	}


	void Renderer::SetShaderFlag(int shaderIndex, ShaderFlags flag)
	{
		if (shaderIndex < shaderList.size())
		{
			shaderList.at(shaderIndex).get()->flags |= flag;
		}
		else throw std::invalid_argument("shaderIndex must be an existing shader");
	}



	void Renderer::Render(std::vector<Body>& bodyList, std::mutex* mutex)
	{
		glEnable(GL_MULTISAMPLE);


		fbo_internal.Use(); // Bind fbo_internal

		glViewport(0, 0, viewport_width, viewport_height);

		glClearColor(ambientLight.getColor()[0] / 2, ambientLight.getColor()[1] / 2, ambientLight.getColor()[2] / 2, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		(*mutex).lock();

		UpdateLights(bodyList);
		UpdateShaders();


		//Draw bodies
		for (auto& body : bodyList)
		{
			body.Draw(*shaderList.at(mainShader).get());
		}

		//Draw orbits
		for (auto& body : bodyList)
		{
			body.orbit.SetColor(body.color);
			body.orbit.Draw(*shaderList.at(orbitShader).get());
		}

		(*mutex).unlock();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Copy fbo_internal to postProcess
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_internal.ID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_postProcess.ID());

		glBlitFramebuffer(0, 0, viewport_width, viewport_height, 0, 0, viewport_width, viewport_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); // Unbin fbo
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		bloom.Apply(fbo_postProcess, fbo_output, viewport_width, viewport_height);
	}


	void Renderer::setViewport(int width, int height)
	{
		viewport_height = height;
		viewport_width = width;

		fbo_internal.Rescale(width, height);
		fbo_postProcess.Rescale(width, height);
		fbo_output.Rescale(width, height);
	}

	void Renderer::UpdateShaders()
	{
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)viewport_width / viewport_height, 0.1f, 1000.0f);

		glm::mat4 view = camera.GetViewMatrix();

		int nbShader = shaderList.size();
		for (int i = 0; i < nbShader; i++)
		{
			Shader& shader = *(shaderList.at(i).get());

			shader.use(); //Make sure to bind the shader

			// Pass camera infos
			if (shader.flags & shaderFlags_NeedCameraInfo)
			{
				shader.setMat4("view", view);
				shader.setMat4("projection", projection);
			}

			// Pass ambient light infos
			if (shader.flags & shaderFlags_NeedAmbientLight)
			{
				shader.setVec3("ambient", ambientLight);
			}

			// Pass point lights infos
			if (shader.flags & shaderFlags_NeedLightPoints)
			{
				shader.setInt("nb_lights", lightPointList.size());
				if (lightPointList.size() > 0)
				{
					char var_pos[] = "light_points[0].position";
					char var_col[] = "light_points[0].color";
					char var_str[] = "light_points[0].strength";

					// Don't pass any value if there isn't
					for (int i = 0; i < lightPointList.size(); i++)
					{
						var_pos[13] = '0' + i;
						var_col[13] = '0' + i;
						var_str[13] = '0' + i;

						shader.setVec3(var_pos, lightPointList.at(i).position);
						shader.setVec3(var_col, lightPointList.at(i).color);
						shader.setFloat(var_str, lightPointList.at(i).strength);
					}
				}
			}


		}
	}

	unsigned int Renderer::GetTexture()
	{
		return fbo_output.GetTexture();
	}

	void Renderer::UpdateLights(std::vector<Body>& bodyList)
	{
		//Update light point
		lightPointList.clear();
		for (auto body : bodyList)
		{
			if (body.isEmissive)
			{
				lightPointList.push_back(body.light);
				lightPointList.back().position = body.position;
			}
		}

	}
}