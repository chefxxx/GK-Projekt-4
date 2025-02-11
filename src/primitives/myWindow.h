//
// Created by Mateusz Mikiciuk on 09/02/2025.
//

#ifndef GK_FINAL_0_MYWINDOW_H
#define GK_FINAL_0_MYWINDOW_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "FlyCamera.h"
#include "LightSource.h"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

class myWindow {
public:
    /* constructor */
    myWindow(const char* title);

    /* basic window functions from tutorial*/
    GLFWwindow* window;
    void RegisterCallbacks();
    void ProcessInput();
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /* camera variables */
    FlyCamera* flyCamera;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    /* pre frame time logic variables and functions */
    void calculateFrameTime();
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    /* scene variables */
    bool day = true;

    /* spotlight */
    LightSource *lSpotlight;
};


#endif //GK_FINAL_0_MYWINDOW_H
