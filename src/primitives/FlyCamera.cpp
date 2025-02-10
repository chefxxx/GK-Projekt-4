//
// Created by Mateusz Mikiciuk on 10/02/2025.
//
#include "FlyCamera.h"
#include "GLFW/glfw3.h"

FlyCamera::FlyCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    mode = STATIC;
    updateCameraVectors();
}

glm::mat4 FlyCamera::GetViewMatrix()
{
    glm::mat4 retMx;
    if (mode == FLY)
        retMx = glm::lookAt(Position, Position + Front, Up);
    if (mode == STATIC)
        retMx = glm::lookAt(STATIC_CAMERA_POSITION, STATIC_CAMERA_TARGET, glm::vec3(0.0f, 1.0f, 0.0f));
    if (mode == STATIC_FOLLOW)
        retMx = glm::lookAt(STATIC_CAMERA_POSITION, followTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    if (mode == OBJECT)
        retMx = glm::lookAt(followCamPos, followTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    return retMx;
}

void FlyCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void FlyCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void FlyCamera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void FlyCamera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}


