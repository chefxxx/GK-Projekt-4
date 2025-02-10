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

    /* shaders, matrices and models */
    Shader testShader("../shaders/test_vert.glsl", "../shaders/test_frag.glsl");

    /* forest model */
    auto fModel = glm::mat4(1.0f);
    fModel = glm::rotate(fModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    fModel = glm::scale(fModel, glm::vec3(1.2f, 1.2f, 0.7f));
    Model forestModel("../resources/models/low_poly_forest_4/scene.gltf", fModel);

    /* backpack */
    stbi_set_flip_vertically_on_load(true);
    auto bModel = glm::mat4(1.0f);
    bModel = glm::translate(bModel, glm::vec3(0.0f, 1.0f, 0.0f));
    bModel = glm::scale(bModel, glm::vec3(0.25f, 0.25f, 0.25f));
    Model backpackModel("../resources/models/backpack/backpack.obj", bModel);

    /* bird */
    stbi_set_flip_vertically_on_load(false);
    auto birdModelMtx = glm::mat4(1.0f);
    auto birdStartPos = glm::vec3(12.0f, 5.0f, 0.0f);
    birdModelMtx = glm::translate(birdModelMtx, birdStartPos);
    Model birdModel("../resources/models/low_poly_bird/scene.gltf", birdModelMtx);

    /* bird test */
    glm::vec4 tmp = glm::vec4(birdStartPos, 1.0f);
    glm::vec4 tmpStartPos = tmp * glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto tmpMtx = glm::mat4(1.0f);
    tmpMtx = glm::translate(tmpMtx, glm::vec3(tmpStartPos.x, tmpStartPos.y, tmpStartPos.z));
    Model birdTest("../resources/models/low_poly_bird/scene.gltf", tmpMtx);

    /* poke_ball */
    stbi_set_flip_vertically_on_load(true);
    auto ballModelMtx = glm::mat4(1.0f);
    auto ballStartPos = glm::vec3(-3.0f, 1.0f, 0.0f);
    ballModelMtx = glm::translate(ballModelMtx, ballStartPos);
    Model ballModel("../resources/models/poke_ball/scene.gltf", ballModelMtx);

    /* sokrates */
    stbi_set_flip_vertically_on_load(false);
    auto sokModelMtx = glm::mat4(1.0f);
    auto sokStartPos = glm::vec3(3.0f, 1.0f, 0.0f);
    sokModelMtx = glm::translate(sokModelMtx, sokStartPos);
    sokModelMtx = glm::rotate(sokModelMtx, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    sokModelMtx = glm::scale(sokModelMtx, glm::vec3(1.5f,1.5f,1.5f));
    Model sokModel("../resources/models/potrait_of_philosopher_sokrates/scene.gltf", sokModelMtx);

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

        testShader.use();

        /* set forest shader */
        testShader.setMat4("projection", projection);
        testShader.setMat4("view", view);
        testShader.setMat4("model", forestModel.model);
        forestModel.Draw(testShader);

        /* set backpack shader */
        testShader.setMat4("projection", projection);
        testShader.setMat4("view", view);
        testShader.setMat4("model", backpackModel.model);
        backpackModel.Draw(testShader);

        /* move bird */
        glm::vec4 tmpBirdPos = glm::vec4(birdStartPos, 1.0f);
        tmpBirdPos = tmpBirdPos * glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));

        /* set for follow camera */
        myWindow.flyCamera->followTarget = glm::vec3(tmpBirdPos.x, tmpBirdPos.y, tmpBirdPos.z);

        birdModelMtx = glm::mat4(1.0f);
        birdModelMtx = glm::translate(birdModelMtx, glm::vec3(tmpBirdPos.x, tmpBirdPos.y, tmpBirdPos.z));
        birdModelMtx = glm::rotate(birdModelMtx, (float)glfwGetTime() * -0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
        birdModelMtx = glm::scale(birdModelMtx, glm::vec3(0.2f, 0.2f, 0.2f));
        birdModel.model = birdModelMtx;

        /* set bird shader */
        testShader.setMat4("model", birdModel.model);
        testShader.setMat4("projection", projection);
        testShader.setMat4("view", view);
        birdModel.Draw(testShader);

        /* test bird */
        glm::vec4 tmpTestPos = tmpStartPos;
        tmpTestPos = tmpTestPos * glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));

        myWindow.flyCamera->look = glm::vec3(tmpTestPos.x, tmpTestPos.y, tmpTestPos.z);

        tmpMtx = glm::mat4(1.0f);
        tmpMtx = glm::translate(tmpMtx, glm::vec3(tmpTestPos.x, tmpTestPos.y, tmpTestPos.z));
        tmpMtx = glm::rotate(tmpMtx, (float)glfwGetTime() * -0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
        tmpMtx = glm::scale(tmpMtx, glm::vec3(0.2f, 0.2f, 0.2f));
        birdTest.model = tmpMtx;

        testShader.setMat4("model", birdTest.model);
        birdTest.Draw(testShader);

        /* ball */
        testShader.setMat4("model", ballModel.model);
        ballModel.Draw(testShader);

        /* sokrates */
        testShader.setMat4("model", sokModel.model);
        sokModel.Draw(testShader);

        // check and call events, swap buffers
        glfwSwapBuffers(myWindow.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}