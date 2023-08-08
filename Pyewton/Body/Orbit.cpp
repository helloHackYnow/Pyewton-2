#include "Orbit.h"


Orbit::Orbit()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_ORBIT_POINT, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Orbit::AppendPoint(glm::vec3 point)
{
    vertexList.insert(vertexList.begin(), point);

    if (vertexList.size() > MAX_ORBIT_POINT)
    {
        vertexList.pop_back();
    }
}

void Orbit::Draw(Shader shader)
{

    if (vertexList.size() > 2)
    {
        glEnable(GL_LINE_SMOOTH);

        shader.use();
        shader.setVec3("Color", orbitColor);


        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexList.size() * sizeof(glm::vec3), &vertexList[0]);

        glDrawArrays(GL_LINE_STRIP, 0, vertexList.size());
    }
    
}

void Orbit::SetColor(Color color_)
{
    orbitColor = color_;
}

void Orbit::Reset()
{
    vertexList.clear();
}
