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

#include "cubemaps.h"
#include "assets.h"
#include "rutils.h"
#include "glapp.h"
#include "camera.h"
#include "gl/shaderprogram.h"
#include "scene/skybox.h"
#include "scene/material.h"

using namespace rgl;
using namespace std;
using namespace glm;
using namespace rutils;

void CubeMaps::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);
    camera.init(settings.fov * DEG_TO_RAD, width/ (float) height, 0.01f, 100.0f);
    camera.moveTo(0, 0, 4);

    trackball.setRadius(double(std::min(width, height)) / 2.0);
    trackball.setCenter(glm::vec2(width, height) / 2.0f);

    initMeshes();
    initTweakBar();
    initCubeMaps();

    skybox = new Skybox(cmaps[settings.activeCMap].highres);

    material = Material(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1),
                        24.f);
    program = ShaderPrograms::reflect;
    pipeline.watchProgram(program);
    pipeline.watchProgram(skybox->getProgram());
}

void CubeMaps::initTweakBar()
{
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(width, height);
    TwBar* bar = TwNewBar("CubeMaps");
    TwDefine(" CubeMaps size='240 320' ");

    // cubemap settings
    TwAddVarRW(bar, "Active CubeMap", TW_TYPE_INT8, &settings.activeCMap, " min=0 max=2 ");
    TwAddVarRW(bar, "Highres cubemap", TW_TYPE_BOOLCPP, &settings.highres, " ");

    // material
    // TwAddVarRW(bar, "mat.ambient", TW_TYPE_COLOR3F, &material.ambient, " colormode = hls ");
    // TwAddVarRW(bar, "mat.diffuse", TW_TYPE_COLOR3F, &material.diffuse, " colormode = hls ");
    TwAddVarRW(bar, "mat.specular", TW_TYPE_COLOR3F, &material.specular, " colormode = hls ");
    TwAddVarRW(bar, "mat.shininess", TW_TYPE_FLOAT, &material.shininess, " min=0 max=2048 step=0.5 ");

    // settings
    //TwAddVarRW(bar, "Background color", TW_TYPE_COLOR4F, &settings.clearColor, "");
    //TwAddVarRW(bar, "Gamma correction", TW_TYPE_BOOLCPP,
    //&settings.gamma, "");
    TwAddVarRW(bar, "Perspective projection", TW_TYPE_BOOLCPP, &settings.perspective, "");
}


void CubeMaps::initMeshes()
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

void CubeMaps::prepare()
{
    GLApp::prepare();
    model->setMaterial(material);

    glClearColor(settings.clearColor.r, settings.clearColor.g,
                 settings.clearColor.b, settings.clearColor.a);

    if (settings.highres)
        model->setTexture(cmaps[settings.activeCMap].highres);
    else
        model->setTexture(getCubeMap(material.shininess));
    skybox->setCubeMap(cmaps[settings.activeCMap].highres);

    int flags = ShaderProgram::BASE;

    if (settings.gamma)
        flags |= ShaderProgram::GAMMA;

    program->use();
    program->setUniformFlags();
}

void CubeMaps::initCubeMaps()
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

void CubeMaps::update()
{
    GLApp::update();
}

void CubeMaps::handleInput()
{
    GLApp::handleInput();
    model->setRotation(trackball.getQuat());
    model->updateMatrices();
}

void CubeMaps::updateCamera()
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

void CubeMaps::render()
{
    glDepthMask(GL_FALSE);
    skybox->render();
    glDepthMask(GL_TRUE);
    scene.render(camera, *program, false);
    TwDraw();
}

void CubeMaps::destroy()
{
    TwTerminate();
}

void CubeMaps::resizeCallback(GLFWwindow* window, int width, int height)
{
    GLApp::resizeCallback(window, width, height);
    TwWindowSize(width, height);
    trackball.setRadius(double(std::min(width, height)) / 2.0);
    trackball.setCenter(glm::vec2(width, height) / 2.0f);
}

void CubeMaps::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLApp::keyCallback(window, key, scancode, action, mods);

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_B) {
            if (settings.tweakBarVisible)
                TwDefine(" CubeMaps visible = false ");
            else
                TwDefine(" CubeMaps visible = true ");
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
    }
}

void CubeMaps::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    TwEventMousePosGLFW(xpos, ypos);

    if (trackball.tracking())
        trackball.move(vec2(xpos, ypos));
}

void CubeMaps::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
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

void CubeMaps::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
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

rgl::CubeMap* CubeMaps::getCubeMap(float shininess)
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
