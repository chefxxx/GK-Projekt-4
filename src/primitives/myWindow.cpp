//
// Created by Mateusz Mikiciuk on 09/02/2025.
//

#include "myWindow.h"

myWindow::myWindow(const char *title)
{
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, nullptr, nullptr);
    flyCamera = new FlyCamera(glm::vec3(0.0f, 0.0f, 3.0f));
}

void myWindow::RegisterCallbacks()
{
    glfwMakeContextCurrent(window);

    auto frameBufferCallback = [](GLFWwindow *window, int width, int height)
    {
        static_cast<myWindow*>(glfwGetWindowUserPointer(window))->framebuffer_size_callback(window, width, height);
    };
    glfwSetFramebufferSizeCallback(window, frameBufferCallback);

    glfwSetWindowUserPointer(window, this);
    auto mouseCallback = [](GLFWwindow *window, double xposIn, double yposIn)
    {
        static_cast<myWindow*>(glfwGetWindowUserPointer(window))->mouse_callback(window, xposIn, yposIn);
    };
    glfwSetCursorPosCallback(window, mouseCallback);

    auto scrollCallback = [](GLFWwindow *window, double xoffset, double yoffset)
    {
        static_cast<myWindow*>(glfwGetWindowUserPointer(window))->scroll_callback(window, xoffset, yoffset);
    };
    glfwSetScrollCallback(window, scrollCallback);
}

void myWindow::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        flyCamera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        flyCamera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        flyCamera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        flyCamera->ProcessKeyboard(RIGHT, deltaTime);
}

void myWindow::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void myWindow::mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    flyCamera->ProcessMouseMovement(xoffset, yoffset);
}

void myWindow::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    flyCamera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void myWindow::calculateFrameTime()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
