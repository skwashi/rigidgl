/**
 * @author Jonas Ransjö
 */

#ifndef CUBES_H
#define CUBES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "glapp.h"
#include "gl/mesh.h"
#include "gl/shaderprogram.h"
#include "gl/gltexture.h"
#include "scene/skybox.h"

class Cubes : public GLApp
{
public:
    virtual void init(int width, int height, const char* title = "Cubes!");
    virtual void render();
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    rgl::VertexBuffer* textVB;
    rgl::Mesh* cubeMesh;
    rgl::ShaderProgram tprogram;
    rgl::ShaderProgram cprogram;
    rgl::ShaderProgram* program;
    rgl::ShaderProgram* textprog;
    rgl::GLTexture* texture;
    Skybox* skybox;
    bool mipmap = true;

};

#endif
