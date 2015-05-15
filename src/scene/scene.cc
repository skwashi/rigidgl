/**
 * @author Jonas Ransjö
 */

#include <vector>

#include "../gl/shaderprogram.h"
#include "../gl/shaderprograms.h"
#include "../gl/renderable.h"
#include "model.h"
#include "light.h"
#include "scene.h"
#include "../camera.h"

using namespace rgl;

void Scene::addModel(Model* model, bool translucent)
{
    if (translucent)
        translucents.push_back(model);
    else
        models.push_back(model);
}

void Scene::addRenderable(rgl::Renderable* renderable, bool translucent)
{
    if (translucent)
        translucents.push_back(renderable);
    else
        renderables.push_back(renderable);
}

void Scene::addLight(Light* light)
{
    lights.push_back(light);
}

void Scene::updateMatrices()
{
    forAll([](Renderable* r){ r->updateMatrices(); });
}

void Scene::updateMatrices(const Camera& camera)
{
    glm::mat4 viewMatrix = camera.getViewMatrix();
    forAll([&viewMatrix](Renderable* r){ r->updateMatrices(viewMatrix); });
}

void Scene::render()
{
    forAll([](Renderable* r){ r->render(); });
}

void Scene::render(ShaderProgram& program, bool lighting)
{
    if (lighting) {
        for (Light* light : lights) {
            light->updateUniforms(program);
            forAll([&program](Renderable* r){ r->render(program); });
        }
    } else {
        forAll([&program](Renderable* r){ r->render(program); });
    }
}

void Scene::render(const Camera& camera,
                   ShaderProgram& program, bool lighting)
{
    program.use();
    if (lighting) {
        bool firstLight = true;
        for (int i = 0, num = lights.size(); i < num; i++) {
            Light* light = lights[i];
            if (!light->isEnabled())
                continue;
            if (firstLight) {
                firstLight = false;
            }
            else
                glBlendFunc(GL_ONE, GL_ONE);
            light->updateUniforms(camera.getTransform(), program);
            forAll([&program](Renderable* r){ if (r->isEnabled()) r->render(program); });
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glDisable(GL_BLEND);
    } else {
        forAll([&program](Renderable* r) {
                if (r->isEnabled())
                    r->render(program);
            });
    }
}

void Scene::renderLights(const Camera& camera, ShaderProgram& program)
{
    program.use();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(false);
    for (Light* light : lights) {
        if (light->isEnabled()) {
            light->updateUniforms(camera.getTransform(), program);
            light->render(program);
        }
    }
    glDepthMask(true);
    glDisable(GL_BLEND);
}

void Scene::forAll(std::function<void(rgl::Renderable*)> f)
{
    for (Model* model : models)
        f(model);
    for (Renderable* r : renderables)
        f(r);
    for (Renderable* t : translucents)
        f(t);
}

bool Scene::applyToModel(size_t i, std::function<void(Model*)> f) {
    if (models.size() > i) {
        f(models[i]);
        return true;
    } else
        return false;
}

bool Scene::applyToLight(size_t i, std::function<void(Light*)> f) {
    if (lights.size() > i) {
        f(lights[i]);
        return true;
    } else
        return false;
}
