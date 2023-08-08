#pragma once
#include <filesystem>
#include <glad/glad.h>
#include <vector>
#include "../../Shader/Graphics.h"
#include "../../Framebuffer.h"


#define DEFAULT_VERTEX "shader_dir/default_v.glsl"

namespace Pyewton::Odin
{
	class Effect
	{
	public:

		Effect();

		void InitVAO();

		// Return the index of the shader in the shaders vector
		virtual void Init();

		virtual void Apply(FrameBuffer& fbo_in, FrameBuffer& fbo_out, int w_width, int w_height);

	public:
		int AddShader(const char* fragmentPath);

		unsigned int VAO;
		unsigned int VBO;

		std::vector<std::unique_ptr<FrameBuffer>> fbos;
		std::vector<Shader> shaders;

		// For keeping trace of the dimensions

		int width;
		int height;
	};
}