//
// Created by Mateusz Mikiciuk on 10/02/2025.
//

#include "LightSource.h"

void LightSource::Draw(Shader shader, glm::mat4 view, glm::mat4 projection)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

LightSource::LightSource(glm::vec3 pos, glm::vec3 col, float cnst, float lin, float quad) {
    position = pos;
    color = col;
    constant = cnst;
    linear = lin;
    quadratic = quad;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTS), VERTS, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


void LightSource::setPoints(float lin, float quad)
{
    linear = lin;
    quadratic = quad;
}

void LightSource::ProcessDirection(LightMovement dir, float time)
{
    float angle = MovSpeed * time;
    auto tmp = glm::vec4(direction, 1.0f);
    if (dir == U)
    {
        tmp = tmp * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (dir == D)
    {
        tmp = tmp * glm::rotate(glm::mat4(1.0f), glm::radians(-angle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (dir == R)
    {
        tmp = tmp * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    if (dir == L)
    {
        tmp = tmp * glm::rotate(glm::mat4(1.0f), glm::radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    direction = glm::normalize(glm::vec3(tmp));
}

float LightSource::GetOuterCutOff()
{
    return glm::cos(glm::radians(outerCutoff));
}

float LightSource::GetInnerCutOff()
{
    return glm::cos(glm::radians(innerCutoff));
}

void LightSource::setDirAndCutoffs(glm::vec3 dir, float value1, float value2)
{
    direction = dir;
    innerCutoff = value1;
    outerCutoff = value2;
}

void LightSource::setDir(glm::vec3 dir)
{
    direction = dir;
}
