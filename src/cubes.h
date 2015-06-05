/**
 * @author Jonas Ransjö
 */

#ifndef CUBES_H
#define CUBES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "glapp.h"
#include "gl/rgl.h"
#include "graphics/batch.h"
#include "scene/skybox.h"

class Cubes : public GLApp
{
public:
    void init(int width, int height, const char* title = "Cubes!");
    void render();
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

    rgraphics::Batch<rgl::Vertex2t>* vbuf;
};

#endif
