//
// Created by Mateusz Mikiciuk on 08/02/2025.
//

#ifndef GK_2_FLYCAMERA_H
#define GK_2_FLYCAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraType
{
    FLY = 0,
    STATIC = 1,
    STATIC_FOLLOW = 2,
    OBJECT = 3
};

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const auto STATIC_CAMERA_TARGET   = glm::vec3(6.0f, 6.0f, 6.0f);
const auto STATIC_CAMERA_POSITION = glm::vec3(15.0f, 15.0f, 15.0f);

const float YAW         =  -180.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class FlyCamera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    CameraType mode;

    FlyCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    /* camera options */
    glm::vec3 followTarget;
private:
    void updateCameraVectors();

};

#endif //GK_2_FLYCAMERA_H
