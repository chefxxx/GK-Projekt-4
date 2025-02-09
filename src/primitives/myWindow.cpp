//
// Created by Mateusz Mikiciuk on 09/02/2025.
//

#include "myWindow.h"

myWindow::myWindow(const char *title)
{
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, nullptr, nullptr);
}

void myWindow::RegisterCallbacks()
{
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, myWindow::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, myWindow::mouse_callback);
    glfwSetScrollCallback(window, myWindow::scroll_callback);
}

void myWindow::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void myWindow::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void myWindow::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

}

void myWindow::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{

}
