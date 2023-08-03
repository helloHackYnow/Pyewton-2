#pragma once 

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ComputeShader : public Shader
{
public:
	ComputeShader();
	ComputeShader(const char* path);
};