#pragma once 

#include "Shader.h"

class GShader : public Shader
{
public:
	GShader();
	GShader(const char* vertexPath, const char* fragmentPath);
};