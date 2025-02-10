//
// Created by Mateusz Mikiciuk on 10/02/2025.
//

#include "LightSource.h"

LightSource::LightSource(glm::vec3 pos, glm::vec3 col)
{
    Position = pos;
    Color = col;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTS), VERTS, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void LightSource::Draw(Shader shader, glm::mat4 view, glm::mat4 projection)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));w
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
