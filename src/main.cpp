#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "primitives//myWindow.h"
#include "primitives/Model.h"
#include "primitives/LightSource.h"

#define FOG_COL_DAY glm::vec3(0.7f, 0.7f, 0.7f)
#define FOG_COL_NIG glm::vec3(0.1f, 0.1f, 0.1f)
#define MAX_FOG 40.0f
#define MIN_FOG 20.0f

void renderQuad();
void prepareGBuff(unsigned int *gPosition, unsigned int *gNormal, unsigned int *gAlbedoSpec, unsigned int *gBuffer);
void drawModels(std::vector<Model>* models, Shader* shader);
void prepareLightShader(Shader *lightShader, myWindow *myWindow, glm::vec3 fogCol,
                        LightSource lPoint1, LightSource lPoint2, LightSource lPoint3, LightSource lPoint4,
                        LightSource lDir, glm::vec4 tmpDirS);
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
    Shader lightShader("../shaders/final_light_vert.glsl", "../shaders/final_light_frag.glsl");

    std::vector<Model> models;

    /* forest model */
    auto fModel = glm::mat4(1.0f);
    fModel = glm::rotate(fModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    fModel = glm::scale(fModel, glm::vec3(1.2f, 1.2f, 0.7f));
    Model forestModel("../resources/models/low_poly_forest_4/scene.gltf", &fModel);
    models.push_back(forestModel);

    /* backpack */
    stbi_set_flip_vertically_on_load(true);
    auto bModel = glm::mat4(1.0f);
    bModel = glm::translate(bModel, glm::vec3(0.0f, 1.0f, 0.0f));
    bModel = glm::scale(bModel, glm::vec3(0.5f, 0.5f, 0.5f));
    Model backpackModel("../resources/models/backpack/backpack.obj", &bModel);
    models.push_back(backpackModel);

    /* bird */
    stbi_set_flip_vertically_on_load(false);
    auto birdModelMtx = glm::mat4(1.0f);
    auto birdStartPos = glm::vec3(12.0f, 5.0f, 0.0f);
    birdModelMtx = glm::translate(birdModelMtx, birdStartPos);
    Model birdModel("../resources/models/low_poly_bird/scene.gltf", &birdModelMtx);
    models.push_back(birdModel);

    /* settings for camera following bird */
    auto cameraPos = glm::vec4(12.0f, 8.0f, 0.0f, 1.0f);
    cameraPos = cameraPos * glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    /* poke_ball */
    stbi_set_flip_vertically_on_load(false);
    auto ballModelMtx = glm::mat4(1.0f);
    auto ballStartPos = glm::vec3(-3.0f, 1.0f, 0.0f);
    ballModelMtx = glm::translate(ballModelMtx, ballStartPos);
    ballModelMtx = glm::scale(ballModelMtx, glm::vec3(0.75f, 0.75f, 0.75f));
    Model ballModel("../resources/models/soccer_ball/scene.gltf", &ballModelMtx);
    models.push_back(ballModel);

    /* sokrates */
    stbi_set_flip_vertically_on_load(false);
    auto sokModelMtx = glm::mat4(1.0f);
    sokModelMtx = glm::translate(sokModelMtx, glm::vec3(3.0f, 1.0f, 0.0f));
    sokModelMtx = glm::rotate(sokModelMtx, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    sokModelMtx = glm::scale(sokModelMtx, glm::vec3(2.5f,2.5f,2.5f));
    Model sokModel("../resources/models/potrait_of_philosopher_sokrates/scene.gltf", &sokModelMtx);
    models.push_back(sokModel);

    /* discobolus */
    auto discModelMtx = glm::mat4(1.0f);
    discModelMtx = glm::translate(discModelMtx, glm::vec3(6.0f, 0.0f, 5.0f));
    discModelMtx = glm::rotate(discModelMtx, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    discModelMtx = glm::rotate(discModelMtx, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    discModelMtx = glm::scale(discModelMtx, glm::vec3(2.5f, 2.5f, 2.5f));
    Model discModel("../resources/models/discobolus/scene.gltf", &discModelMtx);
    models.push_back(discModel);

    /* cleopatra */
    auto cleoMtx = glm::mat4(1.0f);
    cleoMtx = glm::translate(cleoMtx, glm::vec3(7.5f, 0.0f, 0.0f));
    cleoMtx = glm::scale(cleoMtx, glm::vec3(1.5f, 1.5f, 1.5f));
    Model cleoModel("../resources/models/cleopatra/scene.gltf", &cleoMtx);
    models.push_back(cleoModel);

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

    /* deferred shading */
    unsigned int gPosition, gNormal, gAlbedoSpec, gBuffer;
    prepareGBuff(&gPosition, &gNormal, &gAlbedoSpec, &gBuffer);

    // shader configuration
    Shader lightningPassShader("../shaders/lig_deferred_vert.glsl","../shaders/lig_deferred_frag.glsl");
    Shader geometryPassShader("../shaders/final_light_vert.glsl","../shaders/geo_deferred_frag.glsl");
    
    lightningPassShader.use();
    lightningPassShader.setInt("gPosition", 0);
    lightningPassShader.setInt("gNormal", 1);
    lightningPassShader.setInt("gAlbedoSpec", 2);

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

        glClearColor(fogCol.x, fogCol.y, fogCol.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* view and projection matrices */
        glm::mat4 projection;

        if (myWindow.ortho)
        {
            float aspect = (float) SCR_WIDTH / (float) SCR_HEIGHT;
            float left   =  -myWindow.orthoScale * aspect;
            float right  =  myWindow.orthoScale * aspect;
            float bottom =  -myWindow.orthoScale;
            float top    =  myWindow.orthoScale;
            projection =  glm::ortho(left, right, bottom, top, 0.1f, 100.0f);
        }
        else
        {
            projection = glm::perspective(glm::radians(myWindow.flyCamera->Zoom),
                                          (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        }

        glm::mat4 view = myWindow.flyCamera->GetViewMatrix();

        /* prepare move bird, camera and spotlight */
        glm::vec4 tmpBirdPos = glm::vec4(birdStartPos, 1.0f);
        tmpBirdPos = tmpBirdPos *
                     glm::rotate(glm::mat4(1.0f), (float) glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec4 tmpCameraPos =
                cameraPos * glm::rotate(glm::mat4(1.0f), (float) glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));

        /* actually move bird*/
        birdModelMtx = glm::mat4(1.0f);
        birdModelMtx = glm::translate(birdModelMtx, glm::vec3(tmpBirdPos.x, tmpBirdPos.y, tmpBirdPos.z));
        birdModelMtx = glm::rotate(birdModelMtx, (float) glfwGetTime() * -0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
        birdModelMtx = glm::scale(birdModelMtx, glm::vec3(0.2f, 0.2f, 0.2f));
        birdModel.model = &birdModelMtx;

        /* set for follow camera */
        myWindow.flyCamera->followTarget = glm::vec3(tmpBirdPos.x, tmpBirdPos.y, tmpBirdPos.z);
        myWindow.flyCamera->followCamPos = glm::vec3(tmpCameraPos.x, tmpCameraPos.y, tmpCameraPos.z);

        /* spotlight movement */
        myWindow.lSpotlight->position = myWindow.flyCamera->followTarget;
        glm::vec4 tmpDirS = glm::vec4(myWindow.lSpotlight->direction, 1.0f) *
                            glm::rotate(glm::mat4(1.0f), (float) glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));

        if (myWindow.deferredShading)
        {
            /* geometry pass */
            glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            geometryPassShader.use();
            geometryPassShader.setMat4("view", view);
            geometryPassShader.setMat4("projection", projection);
            drawModels(&models, &geometryPassShader);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            /* lightning pass */
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            lightningPassShader.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPosition);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
            prepareLightShader(&lightningPassShader, &myWindow, fogCol, lPoint1, lPoint2, lPoint3, lPoint4,
                               lDir, tmpDirS);
            renderQuad();
        }
        else
        {
            /* draw lights first */
            lPoint1.Draw(testShader, view, projection);
            lPoint2.Draw(testShader, view, projection);
            lPoint3.Draw(testShader, view, projection);
            lPoint4.Draw(testShader, view, projection);

            lightShader.use();
            lightShader.setMat4("view", view);
            lightShader.setMat4("projection", projection);

            prepareLightShader(&lightShader, &myWindow, fogCol, lPoint1, lPoint2, lPoint3, lPoint4,
                               lDir, tmpDirS);

            drawModels(&models, &lightShader);
        }
        // check and call events, swap buffers
        glfwSwapBuffers(myWindow.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void prepareLightShader(Shader *lightShader, myWindow *myWindow, glm::vec3 fogCol,
                        LightSource lPoint1, LightSource lPoint2, LightSource lPoint3, LightSource lPoint4,
                        LightSource lDir, glm::vec4 tmpDirS)
{
    /* shared variables */
    lightShader->use();
    lightShader->setBool("day", myWindow->day);
    lightShader->setVec3("FogCol", fogCol);
    lightShader->setFloat("maxFog", MAX_FOG);
    lightShader->setFloat("minFog", MIN_FOG);
    lightShader->setVec3("viewPos", myWindow->flyCamera->Position);

    /* dir light variables */
    lightShader->setVec3("dirLightDir", lDir.direction);
    lightShader->setVec3("dirLightColor", lDir.color);

    /* spotlight */
    lightShader->setVec3("spotPos", myWindow->lSpotlight->position);
    lightShader->setVec3("spotCol", myWindow->lSpotlight->color);
    lightShader->setVec3("spotDir", glm::vec3(tmpDirS.x, tmpDirS.y, tmpDirS.z));
    lightShader->setFloat("spotInnerCutoff", myWindow->lSpotlight->GetInnerCutOff());
    lightShader->setFloat("spotOuterCutoff", myWindow->lSpotlight->GetOuterCutOff());

    /* p1 light variables */
    lightShader->setVec3("p1Lpos", lPoint1.position);
    lightShader->setVec3("p1Lcol", lPoint1.color);
    lightShader->setFloat("p1Lconst", lPoint1.constant);
    lightShader->setFloat("p1Llinear", lPoint1.linear);
    lightShader->setFloat("p1Lquadratic", lPoint1.quadratic);

    /* p2 light variables */
    lightShader->setVec3("p2Lpos", lPoint2.position);
    lightShader->setVec3("p2Lcol", lPoint2.color);
    lightShader->setFloat("p2Lconst", lPoint2.constant);
    lightShader->setFloat("p2Llinear", lPoint2.linear);
    lightShader->setFloat("p2Lquadratic", lPoint2.quadratic);

    /* p3 light variables */
    lightShader->setVec3("p3Lpos", lPoint3.position);
    lightShader->setVec3("p3Lcol", lPoint3.color);
    lightShader->setFloat("p3Lconst", lPoint3.constant);
    lightShader->setFloat("p3Llinear", lPoint3.linear);
    lightShader->setFloat("p3Lquadratic", lPoint3.quadratic);

    /* p4 light variables */
    lightShader->setVec3("p4Lpos", lPoint4.position);
    lightShader->setVec3("p4Lcol", lPoint4.color);
    lightShader->setFloat("p4Lconst", lPoint4.constant);
    lightShader->setFloat("p4Llinear", lPoint4.linear);
    lightShader->setFloat("p4Lquadratic", lPoint4.quadratic);
}

void drawModels(std::vector<Model>* models, Shader* shader)
{
    for (auto & m: *models)
    {
        shader->setMat3("normalTrans", glm::mat3(glm::transpose(glm::inverse(*m.model))));
        shader->setMat4("model", *m.model);
        m.Draw(*shader);
    }
}

void prepareGBuff(unsigned int *gPosition, unsigned int *gNormal, unsigned int *gAlbedoSpec, unsigned int *gBuffer)
{
    glGenFramebuffers(1, gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *gBuffer);
    // position color buffer
    glGenTextures(1, gPosition);
    glBindTexture(GL_TEXTURE_2D, *gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *gPosition, 0);
    // normal color buffer
    glGenTextures(1, gNormal);
    glBindTexture(GL_TEXTURE_2D, *gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *gNormal, 0);
    // color + specular color buffer
    glGenTextures(1, gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, *gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, *gAlbedoSpec, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}