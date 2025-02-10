//
// Created by Mateusz Mikiciuk on 10/02/2025.
//

#ifndef GK_FINAL_0_LIGHTSOURCE_H
#define GK_FINAL_0_LIGHTSOURCE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

const float VERTS[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
};

class LightSource {
public:
    glm::vec3 Position;
    glm::vec3 Color;
    unsigned int VAO;
    unsigned int VBO;

    float constant;
    float linear;
    float quadratic;

    glm::vec3  direction;
    float cutoff;

    LightSource(glm::vec3 pos, glm::vec3 col, float cnst = 1.0f, float lin = 0.09f, float quad = 0.032f);
    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection);

    void setDirAndCutoff(glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f), float value = 12.5f);
};


#endif //GK_FINAL_0_LIGHTSOURCE_H
