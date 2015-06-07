/**
 * @file modelviewer.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-12
 */

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "modelviewer.h"
#include "assets.h"
#include "rutils.h"
#include "glapp.h"
#include "camera.h"
#include "gl/shaderprogram.h"
#include "gl/framebuffer.h"
#include "scene/skybox.h"
#include "scene/material.h"

using namespace rgl;
using namespace std;
using namespace glm;
using namespace rutils;

void ModelViewer::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);
    camera.init(settings.fov * DEG_TO_RAD, width/ (float) height, 0.01f, 100.0f);
    camera.moveTo(0, 0, 4);

    trackball.setRadius(double(std::min(width, height)) / 2.0);
    trackball.setCenter(glm::vec2(width, height) / 2.0f);

    initMeshes();
    initLights();
    initTweakBar();
    initCubeMaps();

    skybox = new Skybox(cmaps[settings.activeCMap].highres);

    material = DEFAULT_MATERIAL;
    program = ShaderPrograms::mesh;
    pipeline.watchProgram(ShaderPrograms::lightSphere);
    pipeline.watchProgram(program);
    pipeline.watchProgram(skybox->getProgram());
    pipeline.watchProgram(ShaderPrograms::reflectL);
}

void ModelViewer::initTweakBar()
{
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(width, height);
    TwBar* tweakBar = TwNewBar("ModelViewer");
    TwDefine(" ModelViewer size='240 320' ");
    TwBar* cubeMapBar = TwNewBar("CubeMaps");
    TwDefine(" ModelViewer size='240 320' ");

    // // light
    TwAddVarRW(tweakBar, "light.position", TW_TYPE_DIR3F, &settings.lightPosition, "");
    TwAddVarRW(tweakBar, "light.color", TW_TYPE_COLOR3F,
               &light->color, " ");
    TwAddVarRW(tweakBar, "light.attenuation", TW_TYPE_COLOR3F,
               &light->attenuation, " ");
    TwAddVarRW(tweakBar, "light.radius", TW_TYPE_FLOAT,
               &light->radius, " min=0 max=2 step=0.01 ");

    // material
    TwAddVarRW(tweakBar, "mat.ambient", TW_TYPE_COLOR3F, &material.ambient, " colormode = hls ");
    TwAddVarRW(tweakBar, "mat.diffuse", TW_TYPE_COLOR3F, &material.diffuse, " colormode = hls ");
    TwAddVarRW(tweakBar, "mat.specular", TW_TYPE_COLOR3F, &material.specular, " colormode = hls ");
    TwAddVarRW(tweakBar, "mat.shininess", TW_TYPE_FLOAT, &material.shininess, " min=0 max=2048 step=0.125 ");

    // settings
    TwAddVarRW(tweakBar, "Background color", TW_TYPE_COLOR4F, &settings.clearColor, "");
    TwAddVarRW(tweakBar, "Ambient", TW_TYPE_BOOLCPP, &settings.ambient, "");
    TwAddVarRW(tweakBar, "Diffuse", TW_TYPE_BOOLCPP, &settings.diffuse, "");
    TwAddVarRW(tweakBar, "Specular", TW_TYPE_BOOLCPP, &settings.specular, "");
    TwAddVarRW(tweakBar, "Attenuation", TW_TYPE_BOOLCPP, &settings.attenuation, "");
    TwAddVarRW(tweakBar, "Gamma correction", TW_TYPE_BOOLCPP, &settings.gamma, "");
    TwAddVarRW(tweakBar, "Invert colors", TW_TYPE_BOOLCPP, &settings.invertColor, "");
    TwAddVarRW(tweakBar, "Use normal coloring", TW_TYPE_BOOLCPP, &settings.normalColors, "");
    TwAddVarRW(tweakBar, "Perspective projection", TW_TYPE_BOOLCPP, &settings.perspective, "");
    TwAddVarRW(tweakBar, "Show lights", TW_TYPE_BOOLCPP, &settings.renderLights, "");

    // cubemap settings
    TwAddVarRW(cubeMapBar, "Render Skybox", TW_TYPE_BOOLCPP, &settings.renderSkybox, "");
    TwAddVarRW(cubeMapBar, "Active CubeMap", TW_TYPE_INT8, &settings.activeCMap, " min=0 max=2 ");
    TwAddVarRW(cubeMapBar, "Highres cubemap", TW_TYPE_BOOLCPP, &settings.highres, " ");

}

void ModelViewer::initLights()
{
    Node* axis = sceneGraph.allocateNode();
    axis->w = vec3(0, R_PI/8, 0);

    Light * light;

    light = new Light(vec3(1, 1, 1), 1.f, 0.5f, 0.5f, 0.01f);

    light->radius = 2;
    settings.lightPosition = vec3(-5, 5, 5);
    light->moveTo(settings.lightPosition);
    scene.addLight(light);
    this->light = light;

    light = new Light(vec3(0.5f, 0.5f, 0.5f), 0, 0.2f, 1.f, 0.01f);

    light->radius = 2;
    light->attachNode(sceneGraph.allocateNode());
    sceneGraph.attachNode(light->node, axis);
    light->moveTo(-4, 1, 1);

    scene.addLight(light);

    light = new Light(vec3(1, 0, 0), 0, 1, 1, 1);

    light->moveTo(1, 2, 0);
    light->radius = 1;
    scene.addLight(light);
}

void ModelViewer::initMeshes()
{
    meshes[0] = new MeshN();
    meshes[1] = new MeshN();
    meshes[2] = new MeshN();
    meshes[3] = new MeshN();
    meshes[4] = new MeshN();

    int numMeshfiles = 5;
    std::string meshfiles[] = {assets::MODEL_DIR + "armadillo.obj",
                               assets::MODEL_DIR + "bunny.obj",
                               assets::MODEL_DIR + "gargo.obj",
                               assets::MODEL_DIR + "icosphere.obj",
                               assets::MODEL_DIR + "teapot.obj"};

    for (int i = 0; i < numMeshfiles; i++) {
        bool success = loadObj(meshfiles[i], *meshes[i]);
        if (success)
            meshes[i]->bufferData();
        else {
            std::cerr << "Couldn't load mesh in file " << meshfiles[i] << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    model = new Model(meshes[0]);
    scene.addModel(model);
}

void ModelViewer::prepare()
{
    GLApp::prepare();
    model->setMaterial(material);

    glClearColor(settings.clearColor.r, settings.clearColor.g,
                 settings.clearColor.b, settings.clearColor.a);

    light->moveTo(settings.lightPosition);

    if (settings.renderSkybox) {
        if (settings.highres)
            model->setTexture(cmaps[settings.activeCMap].highres);
        else
            model->setTexture(getCubeMap(material.shininess));
        skybox->setCubeMap(cmaps[settings.activeCMap].highres);
    }

    int flags = ShaderProgram::BASE;
    if (settings.ambient)
        flags |= ShaderProgram::AMBIENT;
    if (settings.diffuse)
        flags |= ShaderProgram::DIFFUSE;
    if (settings.specular)
        flags |= ShaderProgram::SPECULAR;
    if (settings.gamma)
        flags |= ShaderProgram::GAMMA;
    if (settings.attenuation)
        flags |= ShaderProgram::ATTENUATION;
    if (settings.invertColor)
        flags |= ShaderProgram::INVERTCOLOR;
    if (settings.normalColors)
        flags |= ShaderProgram::NORMALCOLORS;

    program->setFlags(flags);
    program->use();
    program->setUniformFlags();
}

void ModelViewer::initCubeMaps()
{
    string dirs[3] = {assets::CUBEMAP_DIR + "Forrest/",
                      assets::CUBEMAP_DIR + "LarnacaCastle/",
                      assets::CUBEMAP_DIR + "RomeChurch/"};

    for (int i = 0; i < 3; i++) {
        cmaps[i].highres = rgl::createCubeMapD(dirs[i]);

        cmaps[i].filtered0125 = rgl::createCubeMapD(
            dirs[i] + "prefiltered/0.125/");
        cmaps[i].filtered05 = rgl::createCubeMapD(
            dirs[i] + "prefiltered/0.5/");
        cmaps[i].filtered2 = rgl::createCubeMapD(
            dirs[i] + "prefiltered/2/");
        cmaps[i].filtered8 = rgl::createCubeMapD(
            dirs[i] + "prefiltered/8/");
        cmaps[i].filtered32 = rgl::createCubeMapD(
            dirs[i] + "prefiltered/32/");
        cmaps[i].filtered128 = rgl::createCubeMapD(
            dirs[i] + "prefiltered/128/");
        cmaps[i].filtered512 = rgl::createCubeMapD(
            dirs[i] + "prefiltered/512/");
        cmaps[i].filtered2048 = rgl::createCubeMapD(
            dirs[i] + "prefiltered/2048/");
    }
}

void ModelViewer::update()
{
    GLApp::update();
}

void ModelViewer::handleInput()
{
    GLApp::handleInput();
    model->setRotation(trackball.getQuat());
    model->updateMatrices();
}

void ModelViewer::updateCamera()
{
    camera.setFov(settings.fov * DEG_TO_RAD * settings.zoomFactor);
    camera.update();
    if (settings.perspective) {
        pipeline.update(camera.getProjectionMatrix(), camera.getViewMatrix());
    } else {
        float aspect = camera.getAspectRatio();
        pipeline.update(ortho(-2 * aspect, 2 * aspect, -2.f, 2.f, 0.1f, 20.f), camera.getViewMatrix());
    }
    pipeline.updatePrograms();
}

void ModelViewer::render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (settings.renderSkybox) {
        glDepthMask(GL_FALSE);
        skybox->render();
        glDepthMask(GL_TRUE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1, 1);
        scene.render(camera, *ShaderPrograms::reflectL, false);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glBlendFunc(GL_ONE, GL_ONE);
    }
    scene.render(camera, *program, true);

    if (settings.renderLights)
        scene.renderLights(camera, *ShaderPrograms::lightSphere);


    TwDraw();
}

void ModelViewer::destroy()
{
    TwTerminate();
}

void ModelViewer::resizeCallback(GLFWwindow* window, int width, int height)
{
    GLApp::resizeCallback(window, width, height);
    TwWindowSize(width, height);
    trackball.setRadius(double(std::min(width, height)) / 2.0);
    trackball.setCenter(glm::vec2(width, height) / 2.0f);
}

void ModelViewer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLApp::keyCallback(window, key, scancode, action, mods);

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_B) {
            if (settings.tweakBarVisible)
                TwDefine(" ModelViewer visible = false ");
            else
                TwDefine(" ModelViewer visible = true ");
            settings.tweakBarVisible = !settings.tweakBarVisible;
        }

        if (key == GLFW_KEY_1)
            model->setMesh(meshes[0]);
        if (key == GLFW_KEY_2)
            model->setMesh(meshes[1]);
        if (key == GLFW_KEY_3)
            model->setMesh(meshes[2]);
        if (key == GLFW_KEY_4)
            model->setMesh(meshes[3]);
        if (key == GLFW_KEY_5)
            model->setMesh(meshes[4]);

        if (key == GLFW_KEY_7)
            scene.applyToLight(0, [](Light* light){ light->toggle(); });
        if (key == GLFW_KEY_8)
            scene.applyToLight(1, [](Light* light){ light->toggle(); });
        if (key == GLFW_KEY_9)
            scene.applyToLight(2, [](Light* light){ light->toggle(); });
    }
}

void ModelViewer::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    TwEventMousePosGLFW(xpos, ypos);

    if (trackball.tracking())
        trackball.move(vec2(xpos, ypos));
}

void ModelViewer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (TwEventMouseButtonGLFW(button, action))
        return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    vec2 p = vec2(xpos, ypos);

    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            trackball.setCenter(p);
            trackball.startTracking(p);
        } else if (action == GLFW_RELEASE) {
            trackball.stopTracking();
        }
    }
}

void ModelViewer::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (TwEventMouseWheelGLFW(yoffset))
        return;

    float zoomStep = 1.01;
    if (yoffset > 0)
        zoomStep = 1 / zoomStep;
    else if (yoffset == 0)
        zoomStep = 1;

    settings.zoomFactor *= zoomStep;
    settings.zoomFactor = clamp<float>(settings.zoomFactor, 0, 180/settings.fov);
}

rgl::CubeMap* ModelViewer::getCubeMap(float shininess)
{
    float vals[8] = {0.125f, 0.5f, 2, 8, 32, 128, 512, 2048};
    float s = rm::nearest<float>(shininess, vals, 8);

    CMap activeCMap = cmaps[settings.activeCMap];

    if (s == 0.5f)
        return activeCMap.filtered05;
    if (s == 2)
        return activeCMap.filtered2;
    if (s == 8)
        return activeCMap.filtered8;
    if (s == 32)
        return activeCMap.filtered32;
    if (s == 128)
        return activeCMap.filtered128;
    if (s == 512)
        return activeCMap.filtered512;
    if (s == 2048)
        return activeCMap.filtered2048;

    // else if (s == 0.125f)
    return activeCMap.filtered0125;

}
