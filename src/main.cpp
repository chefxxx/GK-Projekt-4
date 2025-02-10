#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "primitives//myWindow.h"
#include "primitives/Model.h"
#include "primitives/LightSource.h"

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
    Shader lightShader("../shaders/test2_vert.glsl", "../shaders/test3_frag.glsl");

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

    /* settings for camera following bird */
    auto cameraPos = glm::vec4(12.0f, 8.0f, 0.0f, 1.0f);
    cameraPos = cameraPos * glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    /* poke_ball */
    stbi_set_flip_vertically_on_load(false);
    auto ballModelMtx = glm::mat4(1.0f);
    auto ballStartPos = glm::vec3(-3.0f, 1.0f, 0.0f);
    ballModelMtx = glm::translate(ballModelMtx, ballStartPos);
    ballModelMtx = glm::scale(ballModelMtx, glm::vec3(0.5f, 0.5f, 0.5f));
    Model ballModel("../resources/models/soccer_ball/scene.gltf", ballModelMtx);

    /* sokrates */
    stbi_set_flip_vertically_on_load(false);
    auto sokModelMtx = glm::mat4(1.0f);
    auto sokStartPos = glm::vec3(3.0f, 1.0f, 0.0f);
    sokModelMtx = glm::translate(sokModelMtx, sokStartPos);
    sokModelMtx = glm::rotate(sokModelMtx, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    sokModelMtx = glm::scale(sokModelMtx, glm::vec3(1.5f,1.5f,1.5f));
    Model sokModel("../resources/models/potrait_of_philosopher_sokrates/scene.gltf", sokModelMtx);

    /* point lights */
    LightSource lPoint1(glm::vec3(10.0, 5.0, 10.0), glm::vec3(1.0, 1.0, 1.0));
    LightSource lPoint2(glm::vec3(-10.0, 5.0, -10.0), glm::vec3(1.0, 1.0, 1.0));

    /* spotlight */
    LightSource lSpotlight(birdStartPos, glm::vec3(1.0, 1.0, 0.0));
    lSpotlight.setDirAndCutoff();

    /* directional light */
    LightSource lDir(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    lDir.setDirAndCutoff();

    // main window loop
    while(!glfwWindowShouldClose(myWindow.window))
    {
        // pre frame time logic
        myWindow.calculateFrameTime();

        // input
        myWindow.ProcessInput();

        // rendering commands here
        // ...
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* view and projection matrices */
        glm::mat4 projection = glm::perspective(glm::radians(myWindow.flyCamera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = myWindow.flyCamera->GetViewMatrix();

        /* draw lights first */
        lPoint1.Draw(testShader, view, projection);
        lPoint2.Draw(testShader, view, projection);
        lDir.Draw(testShader, view, projection);
        lSpotlight.Draw(testShader, view, projection);

        /* shared variables */
        lightShader.use();
        lightShader.setMat4("view", view);
        lightShader.setVec3("viewPos", myWindow.flyCamera->Position);
        lightShader.setMat4("projection", projection);

//        lightShader.setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
//        lightShader.setVec3("lightPos", lPoint1.Position);

//        lightShader.setFloat("lightConstant", lPoint1.constant);
//        lightShader.setFloat("lightLinear", lPoint1.linear);
//        lightShader.setFloat("lightQuadratic", lPoint1.quadratic);

        /* dir light variables */
        lightShader.setVec3("dirLightDir", lDir.direction);
        lightShader.setVec3("dirLightColor", lDir.color);

        /* set forest shader */
        lightShader.setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(forestModel.model))));
        lightShader.setMat4("model", forestModel.model);
        forestModel.Draw(lightShader);

        /* set backpack shader */
        lightShader.setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(backpackModel.model))));
        lightShader.setMat4("model", backpackModel.model);
        backpackModel.Draw(lightShader);

        /* prepare move bird and camera */
        glm::vec4 tmpBirdPos = glm::vec4(birdStartPos, 1.0f);
        tmpBirdPos = tmpBirdPos * glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec4 tmpCameraPos = cameraPos * glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));

        /* actually move bird*/
        birdModelMtx = glm::mat4(1.0f);
        birdModelMtx = glm::translate(birdModelMtx, glm::vec3(tmpBirdPos.x, tmpBirdPos.y, tmpBirdPos.z));
        birdModelMtx = glm::rotate(birdModelMtx, (float)glfwGetTime() * -0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
        birdModelMtx = glm::scale(birdModelMtx, glm::vec3(0.2f, 0.2f, 0.2f));
        birdModel.model = birdModelMtx;

        /* set for follow camera */
        myWindow.flyCamera->followTarget = glm::vec3(tmpBirdPos.x, tmpBirdPos.y, tmpBirdPos.z);
        myWindow.flyCamera->followCamPos = glm::vec3(tmpCameraPos.x, tmpCameraPos.y, tmpCameraPos.z);

        /* set bird shader */
        lightShader.setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(birdModel.model))));
        lightShader.setMat4("model", birdModel.model);
        birdModel.Draw(lightShader);

        /* ball */
        lightShader.setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(ballModel.model))));
        lightShader.setMat4("model", ballModel.model);
        ballModel.Draw(lightShader);

        /* sokrates */
        lightShader.setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(sokModel.model))));
        lightShader.setMat4("model", sokModel.model);
        sokModel.Draw(lightShader);

        // check and call events, swap buffers
        glfwSwapBuffers(myWindow.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}