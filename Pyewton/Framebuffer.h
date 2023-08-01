#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class FrameBuffer
{
public:
	FrameBuffer(float width = 100, float height = 100);
	~FrameBuffer();
	unsigned int getFrameTexture();
	unsigned int getID();
	void RescaleFrameBuffer(float width, float height);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;
};