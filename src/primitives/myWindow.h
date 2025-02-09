//
// Created by Mateusz Mikiciuk on 09/02/2025.
//

#ifndef GK_FINAL_0_MYWINDOW_H
#define GK_FINAL_0_MYWINDOW_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class myWindow {
public:
    GLFWwindow* window;
    myWindow(const char* title);
    void RegisterCallbacks();
    void ProcessInput();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};


#endif //GK_FINAL_0_MYWINDOW_H
