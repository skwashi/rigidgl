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
        for (Light* light : lights) {
            light->updateUniforms(camera.getTransform(), program);
            for (Model* model : models)
                model->render(program);
        }
    } else {
        for (Model* model : models)
            model->render(program);
    }
}
