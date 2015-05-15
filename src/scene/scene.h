/**
 * @author Jonas Ransjö
 */

#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

#include "../gl/shaderprogram.h"
#include "../gl/pipeline.h"
#include "../gl/renderable.h"
#include "model.h"
#include "node.h"
#include "scenegraph.h"
#include "light.h"
#include "../camera.h"

class Scene
{
public:
    void addModel(Model* model, bool translucent = false);
    void addRenderable(rgl::Renderable* renderable, bool translucent = false);
    void addLight(Light* light);
    void updateMatrices();
    void updateMatrices(const Camera& camera);
    void render();
    void render(rgl::ShaderProgram& program, bool lighting = false);
    void render(const Camera& camera,
                rgl::ShaderProgram& program, bool lighting = false);

    void renderLights(const Camera& camera, rgl::ShaderProgram& program);

    void forAll(std::function<void(rgl::Renderable*)> f);

    bool applyToModel(size_t i, std::function<void(Model*)> f);
    bool applyToLight(size_t i, std::function<void(Light*)> f);

//private:

    std::vector<Model*> models;
    std::vector<rgl::Renderable*> renderables;
    std::vector<rgl::Renderable*> translucents;
    std::vector<Light*> lights;

};

#endif
