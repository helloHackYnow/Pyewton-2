#include "Orbit.h"

Orbit::Orbit(Shader orbitShader)
{
	shader = orbitShader;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Orbit::DrawOrbit(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
    shader.use();
    shader.setVec3("Color", orbitColor);
    
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(glm::vec3), &vertexList[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(VAO);

    glDrawArrays(GL_LINE_STRIP, 0, vertexList.size());

    glBindVertexArray(0);
}
