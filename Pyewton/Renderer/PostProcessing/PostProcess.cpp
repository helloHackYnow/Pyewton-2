#include "PostProcess.h"
#include <iostream>

namespace fs = std::filesystem;

Effect::Effect()
{
	fs::path current_dir = fs::current_path();
	fs::path compute("shader_dir/Bloom.glsl");
	fs::path full = current_dir / compute;

	shader = ComputeShader(full.string().data());
}

void Effect::Use(FrameBuffer fbo, int v_width, int v_height)
{
	glBindImageTexture(4, fbo.GetTexture(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);

	shader.use();

	glDispatchCompute((unsigned int)v_width, (unsigned int)v_height, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	std::cout << "bloom\n";
}
