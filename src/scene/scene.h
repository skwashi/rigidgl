/**
 * @author Jonas Ransjö
 */

#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../gl/shaderprogram.h"
#include "../gl/pipeline.h"
#include "model.h"
#include "node.h"
#include "scenegraph.h"


class Scene
{
public:
    void addModel(Model* model);
    void updateMatrices();
    void render();
    void render(rgl::ShaderProgram& program);

private:
    std::vector<Model*> models;
};

#endif
