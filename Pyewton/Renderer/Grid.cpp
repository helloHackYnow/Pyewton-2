#include "Grid.h"

Grid::Grid()
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions 
		 1.0f,  1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f,

		-1.0f, -1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f,
	};
	//Create VAO & VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
}

void Grid::Draw(Shader shader) 
{
	glUseProgram(shader.ID); // Use your compiled shader program
	glEnable(GL_BLEND);

	// Set your uniform values, if needed

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6); // Draw the quad

	glDisable(GL_BLEND);
	glBindVertexArray(0);

	glUseProgram(0);
}
