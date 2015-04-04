#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include "math/rm.h"
#include "math/rmath.h"
#include "math/tr3.h"
#include "camera.h"

#include <GLFW/glfw3.h>


using namespace std;
using namespace rm;
using namespace glm;

static void error_callback(int error, const char* desc) 
{
    fputs(desc, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char* argv[])
{

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Title", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    while(!glfwWindowShouldClose(window)) {
        // keep running
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        double time = glfwGetTime();
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1, 1, 1, -1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) time * 50, 0, 0, 1);

        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0, 1, 0);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0, 0, 1);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
