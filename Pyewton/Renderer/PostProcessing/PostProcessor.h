#pragma once

#include <glad/glad.h>
#include "Effects/Effect.h"
#include "../Framebuffer.h"

namespace Pyewton::Odin
{

	class PostProcessor
	{
	public:
		PostProcessor();
		void Init();

		void Use(Effect& effect, FrameBuffer& fbo_in, FrameBuffer& fbo_out);

	private:
		unsigned int VAO;
		unsigned int VBO;
	};

}