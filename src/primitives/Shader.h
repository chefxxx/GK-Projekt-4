//
// Created by Mateusz Mikiciuk on 04/02/2025.
//

#ifndef GK4_TRY_1_SHADER_H
#define GK4_TRY_1_SHADER_H

#include "glad/include/glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};


#endif //GK4_TRY_1_SHADER_H
