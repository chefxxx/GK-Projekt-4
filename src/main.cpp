#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "primitives//myWindow.h"
#include "primitives/Model.h"

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    myWindow myWindow("GK4");

    if (myWindow.window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    myWindow.RegisterCallbacks();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* gl variable setting */
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(myWindow.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* shaders and models */
    Shader testShader("../shaders/test_vert.glsl", "../shaders/test_frag.glsl");
    stbi_set_flip_vertically_on_load(true);
    Model testModel("../resources/models/backpack/backpack.obj");

    // main window loop
    while(!glfwWindowShouldClose(myWindow.window))
    {
        // pre frame time logic
        myWindow.calculateFrameTime();

        // input
        myWindow.ProcessInput();

        // rendering commands here
        // ...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //TODO: here will be functions that return matrices depending on a camera
        /* eg. myWindow.GetPerspectiveMatrix
        *  eg. myWindow.GetViewMatrix        */

        glm::mat4 projection = glm::perspective(glm::radians(myWindow.flyCamera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = myWindow.flyCamera->GetViewMatrix();
        auto model = glm::mat4(1.0f);

        testShader.use();
        testShader.setMat4("projection", projection);
        testShader.setMat4("view", view);
        testShader.setMat4("model", model);

        testModel.Draw(testShader);

        // check and call events, swap buffers
        glfwSwapBuffers(myWindow.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}