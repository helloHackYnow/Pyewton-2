#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Init(int _width, int _height, bool isHDR = false, int samples = 1);
	void Rescale(int _width, int _height);
	void Use();

	unsigned int GetTexture();
	unsigned int ID();

	bool IsMultiSamples();


private:
	unsigned int fboID;
	unsigned int texID;
	unsigned int rboID;

	int samples;

};