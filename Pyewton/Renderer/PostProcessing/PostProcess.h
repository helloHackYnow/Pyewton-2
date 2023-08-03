#pragma once

#include "../Shader/Compute.h"
#include "../../Renderer/Framebuffer.h"

#include <glad/glad.h>

#include <filesystem>



class Effect
{
public:
	Effect();

	void Use(FrameBuffer fbo, int v_width, int v_height);

private:
	ComputeShader shader;

};