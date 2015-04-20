/**
 * @author Jonas Ransjö
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glapp.h"
#include "gl/mesh.h"
#include "gl/shaderprogram.h"

class Cubes : public GLApp
{
public:
    virtual void init(int width, int height, const char* title = "Cubes!");
    virtual void render();
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    std::shared_ptr<rgl::Mesh> cubeMesh;
    rgl::ShaderProgram program;
};

