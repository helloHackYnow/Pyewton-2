#pragma once

#include "Shader/Graphics.h"
#include <glad/glad.h>

class Grid
{
public:
	Grid();

	void Draw(Shader shader);

private:

	unsigned int VAO;
	unsigned int VBO;



};