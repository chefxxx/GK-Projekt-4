#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "primitives//myWindow.h"
#include "primitives/Model.h"
#include "primitives/LightSource.h"

#define FOG_COL_DAY glm::vec3(0.7f, 0.7f, 0.7f)
#define FOG_COL_NIG glm::vec3(0.1f, 0.1f, 0.1f)
#define MAX_FOG 40.0f
#define MIN_FOG 20.0f

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
    bModel = glm::scale(bModel, glm::vec3(0.5f, 0.5f, 0.5f));
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
    ballModelMtx = glm::scale(ballModelMtx, glm::vec3(0.75f, 0.75f, 0.75f));
    Model ballModel("../resources/models/soccer_ball/scene.gltf", ballModelMtx);

    /* sokrates */
    stbi_set_flip_vertically_on_load(false);
    auto sokModelMtx = glm::mat4(1.0f);
    sokModelMtx = glm::translate(sokModelMtx, glm::vec3(3.0f, 1.0f, 0.0f));
    sokModelMtx = glm::rotate(sokModelMtx, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    sokModelMtx = glm::scale(sokModelMtx, glm::vec3(2.5f,2.5f,2.5f));
    Model sokModel("../resources/models/potrait_of_philosopher_sokrates/scene.gltf", sokModelMtx);

    /* discobolus */
    auto discModelMtx = glm::mat4(1.0f);
    discModelMtx = glm::translate(discModelMtx, glm::vec3(6.0f, 0.0f, 5.0f));
    discModelMtx = glm::rotate(discModelMtx, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    discModelMtx = glm::rotate(discModelMtx, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    discModelMtx = glm::scale(discModelMtx, glm::vec3(2.5f, 2.5f, 2.5f));
    Model discModel("../resources/models/discobolus/scene.gltf", discModelMtx);

    /* point lights */
    LightSource lPoint1(glm::vec3(10.0, 5.0, 10.0), glm::vec3(1.0, 1.0, 1.0));
    lPoint1.setPoints(0.14f, 0.07f);
    LightSource lPoint2(glm::vec3(-8.0, 5.0, -8.0), glm::vec3(1.0, 1.0, 1.0));
    lPoint2.setPoints(0.07f, 0.017f);
    LightSource lPoint3(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(1.0, 1.0, 1.0));
    LightSource lPoint4(glm::vec3(5.0f, 1.0f, 1.0f), glm::vec3(1.0, 1.0, 1.0));

    /* spotlight */
    myWindow.lSpotlight = new LightSource (birdStartPos, glm::vec3(1.0, 1.0, 1.0));
    myWindow.lSpotlight->setDirAndCutoffs(glm::vec3(0.0f, -1.0f, 0.0f), 20.0f, 25.0f);

    /* directional light */
    LightSource lDir(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    lDir.setDir();

    /* fog */
    glm::vec3 fogCol = FOG_COL_DAY;

    // main window loop
    while(!glfwWindowShouldClose(myWindow.window))
    {
        // pre frame time logic
        myWindow.calculateFrameTime();

        // input
        myWindow.ProcessInput();

        // rendering commands here
        // ...

        if(myWindow.day)
            fogCol = FOG_COL_DAY;
        else
            fogCol = FOG_COL_NIG;
        glClearColor(fogCol.x, fogCol.y ,fogCol.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* view and projection matrices */
        glm::mat4 projection = glm::perspective(glm::radians(myWindow.flyCamera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = myWindow.flyCamera->GetViewMatrix();

        /* prepare move bird, camera and spotlight */
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

        /* spotlight movement */
        myWindow.lSpotlight->position = myWindow.flyCamera->followTarget;
        glm::vec4 tmpDirS = glm::vec4(myWindow.lSpotlight->direction, 1.0f) * glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));

        /* draw lights first */
        lPoint1.Draw(testShader, view, projection);
        lPoint2.Draw(testShader, view, projection);
        lPoint3.Draw(testShader, view, projection);
        lPoint4.Draw(testShader, view, projection);

        /* shared variables */
        lightShader.use();
        lightShader.setBool("day", myWindow.day);
        lightShader.setVec3("FogCol", fogCol);
        lightShader.setFloat("maxFog", MAX_FOG);
        lightShader.setFloat("minFog", MIN_FOG);
        lightShader.setMat4("view", view);
        lightShader.setVec3("viewPos", myWindow.flyCamera->Position);
        lightShader.setMat4("projection", projection);

        /* dir light variables */
        lightShader.setVec3("dirLightDir", lDir.direction);
        lightShader.setVec3("dirLightColor", lDir.color);

        /* spotlight */
        lightShader.setVec3("spotPos", myWindow.lSpotlight->position);
        lightShader.setVec3("spotCol", myWindow.lSpotlight->color);
        lightShader.setVec3("spotDir", glm::vec3(tmpDirS.x, tmpDirS.y, tmpDirS.z));
        lightShader.setFloat("spotInnerCutoff", myWindow.lSpotlight->GetInnerCutOff());
        lightShader.setFloat("spotOuterCutoff", myWindow.lSpotlight->GetOuterCutOff());

        /* p1 light variables */
        lightShader.setVec3("p1Lpos", lPoint1.position);
        lightShader.setVec3("p1Lcol", lPoint1.color);
        lightShader.setFloat("p1Lconst", lPoint1.constant);
        lightShader.setFloat("p1Llinear", lPoint1.linear);
        lightShader.setFloat("p1Lquadratic", lPoint1.quadratic);

        /* p2 light variables */
        lightShader.setVec3("p2Lpos", lPoint2.position);
        lightShader.setVec3("p2Lcol", lPoint2.color);
        lightShader.setFloat("p2Lconst", lPoint2.constant);
        lightShader.setFloat("p2Llinear", lPoint2.linear);
        lightShader.setFloat("p2Lquadratic", lPoint2.quadratic);

        /* p3 light variables */
        lightShader.setVec3("p3Lpos", lPoint3.position);
        lightShader.setVec3("p3Lcol", lPoint3.color);
        lightShader.setFloat("p3Lconst", lPoint3.constant);
        lightShader.setFloat("p3Llinear", lPoint3.linear);
        lightShader.setFloat("p3Lquadratic", lPoint3.quadratic);

        /* p4 light variables */
        lightShader.setVec3("p4Lpos", lPoint4.position);
        lightShader.setVec3("p4Lcol", lPoint4.color);
        lightShader.setFloat("p4Lconst", lPoint4.constant);
        lightShader.setFloat("p4Llinear", lPoint4.linear);
        lightShader.setFloat("p4Lquadratic", lPoint4.quadratic);

        /* set forest shader */
        lightShader.setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(forestModel.model))));
        lightShader.setMat4("model", forestModel.model);
        forestModel.Draw(lightShader);

        /* set backpack shader */
        lightShader.setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(backpackModel.model))));
        lightShader.setMat4("model", backpackModel.model);
        backpackModel.Draw(lightShader);

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

        /* discobolus */
        lightShader.setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(discModel.model))));
        lightShader.setMat4("model", discModel.model);
        discModel.Draw(lightShader);

        // check and call events, swap buffers
        glfwSwapBuffers(myWindow.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}