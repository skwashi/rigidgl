/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "glapp.h"
#include "gl/vertexattrib.h"
#include "gl/vertexarray.h"
#include "gl/glutils.h"
#include "math/rmath.h"
#include "camera.h"

using namespace std;
using namespace rgl;

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

    // temp stuff for testing

    VertexAttrib attribs[2] = {VA_POS3, VA_COLOR};

    vertexArray = new VertexArray(4, 6, attribs, 2);

    // vertexArray->vertices = {
    //     -0.5f, 0.5f, 0.0f,
    //     1, 0, 0, 1,
    //     -0.5f,-0.5f, 0.0f,
    //     0, 1, 0, 1,
    //     0.5f,-0.5f, 0.0f,
    //     0, 0, 1, 1,
    //     0.5f, 0.5f, 0.0f,
    //     0.5, 0.5, 0, 1
    // };

    vertexArray->vertices = {
        -50.0f, 50.0f, 0.0f,
        1, 0, 0, 1,
        -50.0f,-50.0f, 0.0f,
        0, 1, 0, 1,
        50.0f,-50.0f, 0.0f,
        0, 0, 1, 1,
        50.0f, 50.0f, 0.0f,
        5, 5, 0, 1
    };

    vertexArray->indices = {
        0, 1, 2,
        2, 3, 0
    };

    vertexArray->bind();
    vertexArray->bufferData(GL_STATIC_DRAW);
    vertexArray->unbind();

    string dir = "/home/ashi/src/cpp/projects/rigid/src/shaders/";
    string vertSrc = readFile((dir + "standard.vert").c_str());
    string fragSrc = readFile((dir + "standard.frag").c_str());

    program.create(vertSrc, fragSrc, attribs, 2);
    camera.init(45.0f, width/ (float) height, 0.1f, 1000.0f);
    camera.translate(0, 0, 100);
    program.use();
    program.setUniformMatrix4f(U_PROJMATRIX, camera.getProjectionMatrix());
    program.disable();
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
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
}

void GLApp::updateFPS()
{
    if (time - timeStamp > 1) {
        fps = frameCount;
        frameCount = 0;
        timeStamp = time;
    }
    frameCount++;
}

void GLApp::prepare()
{
}

void GLApp::handleInput()
{
    double xp, yp;
    glfwGetCursorPos(window, &xp, &yp);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (xp != xpos) {
            camera.rotateLocally(-R_PI * 20 * dt * (xp - xpos) / (float) width, 0, 1, 0);
        }
        if (yp != ypos) {
            camera.rotateLocally(-R_PI * 20 * dt * (yp - ypos) / (float) width, 1, 0, 0);
        }
        glfwSetCursorPos(window, xp, yp);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    xpos = xp;
    ypos = yp;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.translateLocally(0, 10 * dt, 0);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.translateLocally(0, -10 * dt, 0);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.translateLocally(-10 * dt, 0, 0);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.translateLocally(10 * dt, 0, 0);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.translateLocally(0, 0, -10 * dt);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.translateLocally(0, 0, 10 * dt);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.rotateLocally(R_PI * dt, 0, 0, 1);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.rotateLocally(-R_PI * dt, 0, 0, 1);
}

void GLApp::update()
{
    camera.update();
}

void GLApp::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.use();
    program.setUniformMatrix4f(U_PROJMATRIX, camera.getProjectionMatrix());
    program.setUniformMatrix4f(U_VIEWMATRIX, camera.getViewMatrix());
    vertexArray->bind();
    vertexArray->drawElements(GL_TRIANGLES);
    vertexArray->unbind();
    program.disable();
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
