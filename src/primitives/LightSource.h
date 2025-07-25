//
// Created by Mateusz Mikiciuk on 10/02/2025.
//

#ifndef GK_FINAL_0_LIGHTSOURCE_H
#define GK_FINAL_0_LIGHTSOURCE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

enum LightMovement
{
    U,
    D,
    L,
    R,
};

const float MovSpeed = 15.0f;

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
    glm::vec3 prevPosition;
    glm::vec3 position;
    glm::vec3 color;
    unsigned int VAO;
    unsigned int VBO;

    float constant;
    float linear;
    float quadratic;

    glm::vec3  direction;
    float innerCutoff;
    float outerCutoff;

    LightSource(glm::vec3 pos, glm::vec3 col, float cnst = 1.0f, float lin = 0.09f, float quad = 0.032f);
    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection);

    void setDirAndCutoffs(glm::vec3 dir = glm::vec3(0.0f, -1.0f, 0.0f), float value1 = 12.5f, float value2 = 20.0f);
    void setPoints(float lin, float quad);
    void ProcessDirection(LightMovement dir, float time);
    void setDir(glm::vec3 dir = glm::vec3(0.0f, -1.0f, 0.0f));
    float GetInnerCutOff();
    float GetOuterCutOff();
};


#endif //GK_FINAL_0_LIGHTSOURCE_H
