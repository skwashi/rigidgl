/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <stdlib.h>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "glapp.h"
#include "gl/vertexattrib.h"
#include "math/rmath.h"
#include "camera.h"

#include "gl/mesh.h"
#include "scene/node.h"
#include "scene/scenegraph.h"

using namespace std;
using namespace rgl;
using namespace glm;

InputHandler* InputHandler::handler;

void GLApp::init(int width, int height, const char* title)
{
    this->title = title;
    this->width = width;
    this->height = height;
    initGlfw();
    initGLEW();
    initGL();
    state = INIT;
}

void GLApp::run()
{
    state = RUNNING;
    initTime();
    while (state != STOPPED && !glfwWindowShouldClose(window)) {
        updateTime();
        updateFPS();
        prepare();
        glfwPollEvents();
        handleInput();
        updateCamera();
        if (state != PAUSED)
            update();
        render();
        glfwSwapBuffers(window);
        cleanUp();
    }
    destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLApp::stop()
{
    state = STOPPED;
}

void GLApp::initGlfw()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    initHandler();
    glfwSetErrorCallback(InputHandler::errorCallbackDispatcher);
    glfwSetKeyCallback(window, InputHandler::keyCallbackDispatcher);
    glfwSetWindowSizeCallback(window, InputHandler::resizeCallbackDispatcher);
    glfwSetCursorPosCallback(window, InputHandler::cursorCallbackDispatcher);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
}

void GLApp::initGLEW()
{
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(status) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void GLApp::initGL()
{
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLApp::initTime()
{
    timeStamp = prevTime = time = glfwGetTime();
    fps = frameCount = 0;
    dt = 0;
}

void GLApp::updateTime()
{
    prevTime = time;
    time = glfwGetTime();
    dt = time - prevTime;
    if (state != PAUSED)
        runningTime += dt;
}

void GLApp::updateFPS()
{
    if (time - timeStamp > 1) {
        fps = frameCount;
        frameCount = 0;
        timeStamp = time;
        std::cout << "FPS: " << fps << std::endl;
    }
    frameCount++;
}

void GLApp::prepare()
{
}

void GLApp::handleInput()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (xpos != cursor.x) {
            camera.rotateLocally(-R_PI * 20 * dt * (xpos - cursor.x) / (float) width, 0, 1, 0);
        }
        if (ypos != cursor.y) {
            camera.rotateLocally(-R_PI * 20 * dt * (ypos - cursor.y) / (float) width, 1, 0, 0);
        }
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    cursor.x = xpos;
    cursor.y = ypos;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.translateLocally(0, 20 * dt, 0);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.translateLocally(0, -20 * dt, 0);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.translateLocally(-20 * dt, 0, 0);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.translateLocally(20 * dt, 0, 0);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.translateLocally(0, 0, -20 * dt);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.translateLocally(0, 0, 20 * dt);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.rotateLocally(R_PI * dt, 0, 0, 1);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.rotateLocally(-R_PI * dt, 0, 0, 1);
}

void GLApp::updateCamera()
{
    camera.update();
    pipeline.update(camera.getProjectionMatrix(), camera.getViewMatrix());
    pipeline.updatePrograms();
}

void GLApp::update()
{
    sceneGraph.updateTransforms(dt);
    scene.updateMatrices();
}

void GLApp::render()
{
}

void GLApp::cleanUp()
{
}

void GLApp::destroy()
{
}

void GLApp::errorCallback(int error, const char* desc)
{
    cerr << desc;
}

void GLApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (state == RUNNING)
            state = PAUSED;
        else if (state == PAUSED)
            state = RUNNING;
    }
}

void GLApp::resizeCallback(GLFWwindow* window, int width, int height)
{
    this->width = width;
    this->height = height;
    camera.setAspectRatio(width / (float) height);
    glViewport(0, 0, width, height);
}

void GLApp::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
}
