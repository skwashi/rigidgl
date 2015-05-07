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

class Lighting : public GLApp
{
public:
    virtual void init(int width, int height, const char* title = "Cubes!");
    virtual void render();

private:
    rgl::ShaderProgram* program = NULL;
};

#endif
