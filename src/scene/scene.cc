/**
 * @author Jonas Ransjö
 */

#include <vector>

#include "../gl/shaderprogram.h"
#include "model.h"
#include "light.h"
#include "scene.h"
#include "../camera.h"

void Scene::addModel(Model* model)
{
    models.push_back(model);
}

void Scene::addLight(Light* light)
{
    lights.push_back(light);
}

void Scene::updateMatrices()
{
    for (Model* model : models)
        model->updateMatrices();
}

void Scene::render()
{
    for (Model* model : models)
        model->render();
}

void Scene::render(rgl::ShaderProgram& program, bool lighting)
{
    if (lighting) {
        for (Light* light : lights) {
            light->updateUniforms(program);
            for (Model* model : models)
                model->render(program);
        }
    } else {
        for (Model* model : models)
            model->render(program);
    }
}

void Scene::render(const Camera& camera,
                   rgl::ShaderProgram& program, bool lighting)
{
    if (lighting) {
        for (int i = 0, num = lights.size(); i < num; i++) {
            Light* light = lights[i];
            if (i == 0)
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            else
                glBlendFunc(GL_ONE, GL_ONE);
            light->updateUniforms(camera.getTransform(), program);
            for (Model* model : models)
                model->render(program);
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        for (Model* model : models)
            model->render(program);
    }
}
