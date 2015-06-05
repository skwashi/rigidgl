/**
 * @file lighting.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */

#ifndef LIGHTING_H
#define LIGHTING_H

#include "glapp.h"
#include "gl/shaderprogram.h"
#include "gl/vertexbuffer.h"
#include "gl/framebuffer.h"

class Lighting : public GLApp
{
public:
    void init(int width, int height, const char* title = "Cubes!");
    void render();

private:

    rgl::ShaderProgram* program = NULL;
    rgl::ShaderProgram* qprogram = NULL;

};

#endif
