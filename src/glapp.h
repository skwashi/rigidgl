/**
 * @author Jonas Ransjö
 */

#ifndef GLAPP_H
#define GLAPP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gl/pipeline.h"
#include "gl/shaderprogram.h"
#include "camera.h"
#include "scene/scenegraph.h"
#include "scene/scene.h"
#include "module.h"

class InputHandler
{
protected:
    virtual void keyCallback(GLFWwindow* window,
                             int key, int scancode,
                             int action, int mods) = 0;
    virtual void errorCallback(int error, const char* desc) = 0;
    virtual void resizeCallback(GLFWwindow* window, int width, int height) = 0;
    virtual void cursorCallback(GLFWwindow* window, double xpos, double ypos) = 0;
    virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) = 0;
    virtual void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) = 0;

    static InputHandler* handler;

    virtual void initHandler()
    {
        handler = this;
    }

    static void errorCallbackDispatcher(int error, const char* desc)
    {
        if (handler)
            handler->errorCallback(error, desc);
    }

    static void keyCallbackDispatcher(GLFWwindow* window,
                                      int key, int scancode,
                                      int action, int mods)
    {
        if (handler)
            handler->keyCallback(window, key, scancode, action, mods);
    }

    static void resizeCallbackDispatcher(GLFWwindow* window,
                                         int width, int height)
    {
        if (handler)
            handler->resizeCallback(window, width, height);
    }

    static void cursorCallbackDispatcher(GLFWwindow* window,
                                         double xpos, double ypos)
    {
        if (handler)
            handler->cursorCallback(window, xpos, ypos);
    }

    static void mouseButtonCallbackDispatcher(GLFWwindow* window,
                                              int button, int action, int mods)
    {
        if (handler)
            handler->mouseButtonCallback(window, button, action, mods);
    }

    static void scrollCallbackDispatcher(GLFWwindow* window,
                                         double xoffset, double yoffset)
    {
        if (handler)
            handler->scrollCallback(window, xoffset, yoffset);
    }

};

class GLApp : InputHandler
{
public:
    friend class Module;

    enum State {
        INIT = 0,
        RUNNING = 1,
        PAUSED = 2,
        STOPPED = 3
    };

    GLApp()
    {
    }

    ~GLApp()
    {
        if (window) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }

    virtual void init(int width, int height,
                      const char* title = "No Title");
    void run();
    virtual void stop();

    void addModule(Module* module);

protected:

    void initGlfw();
    void initGLEW();
    virtual void initGL();
    void initPrograms();
    void initTime();
    void updateTime();
    void updateFPS();
    virtual void prepare();
    virtual void handleInput();
    virtual void updateCamera();
    virtual void update();
    virtual void prerender();
    virtual void render();
    virtual void cleanUp();
    virtual void destroy();

    virtual void errorCallback(int error, const char* desc);
    virtual void resizeCallback(GLFWwindow* window, int width, int height);
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    virtual void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    virtual void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    State state;
    int width;
    int height;
    std::string title;
    GLFWwindow* window;
    glm::dvec2 cursor;

    double time; // time in seconds since init
    double prevTime; // time at last frame
    double dt; // time - prevTime
    double runningTime; // time while not paused

    int frameCount;
    int fps;
    double timeStamp;

    Camera camera;
    rgl::Pipeline pipeline;
    SceneGraph sceneGraph;
    Scene scene;

    std::vector<Module*> modules;
};

#endif
