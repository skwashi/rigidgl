// #include <iostream>
// #include <vector>
// #include <glm/glm.hpp>
// #include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/string_cast.hpp>
// #include <stdlib.h>
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>

#include "glapp.h"

int main(int argc, char* argv[])
{

    GLApp* glApp = new GLApp;
    glApp->init(640, 480);
    glApp->run();

    exit(EXIT_SUCCESS);
}
