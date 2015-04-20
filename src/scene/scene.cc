/**
 * @author Jonas Ransjö
 */

#include <vector>

#include "../gl/shaderprogram.h"
#include "model.h"
#include "scene.h"

void Scene::addModel(Model* model)
{
    models.push_back(model);
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

void Scene::render(rgl::ShaderProgram& program)
{
    for (Model* model : models)
        model->render(program);
}
