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
#include "light.h"
#include "../camera.h"

class Scene
{
public:
    void addModel(Model* model);
    void addLight(Light* light);
    void updateMatrices();
    void render();
    void render(rgl::ShaderProgram& program, bool lighting = false);
    void render(const Camera& camera,
                rgl::ShaderProgram& program, bool lighting = false);

//private:
    std::vector<Model*> models;
    std::vector<Light*> lights;
};

#endif
