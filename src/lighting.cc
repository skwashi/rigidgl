/**
 * @file lighting.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */


#include <GL/glew.h>

#include "lighting.h"
#include "assets.h"
#include "rutils.h"
#include "gl/rgl.h"

using namespace std;
using namespace rgl;
using namespace rutils;
using glm::vec2;
using glm::vec3;
using glm::vec4;

void Lighting::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);
    camera.init(45.0f, width/ (float) height, 0.01f, 200.0f);
    camera.moveTo(0, 0, 5);

    ShaderProgram* cprogram = createShader("normcol", VAS_POSNORMCOL);
    if (cprogram)
        pipeline.watchProgram(*cprogram);

    program = createShader("mesh", VAS_POSNORM);
    if (program)
        pipeline.watchProgram(program);

    Mesh* mesh = new MeshN();
    bool success = loadObj(assets::MODEL_DIR + "teapot.obj", *mesh);

    if (success) {
        mesh->bufferData();
        Model* model = new Model(sceneGraph.allocateNode(), mesh);
        model->node->w = vec3(0, -R_PI/2, 0);
        model->attachProgram(cprogram);
        scene.addModel(model);
    }

    mesh = new MeshN();
    success = loadObj(assets::MODEL_DIR + "armadillo.obj", *mesh);

    if (success) {
        mesh->bufferData();
        Model* model = new Model(sceneGraph.allocateNode(), mesh);
        model->moveTo(1, 0, 1);
        model->attachProgram(cprogram);
        scene.addModel(model);
    }

    Light* light = new Light(vec3(0.0001, 0.0001, 0.0001),
                             vec3(0.1f, 0.1f, 0.5f),
                             vec3(0.1f, 0.1f, 0.1f),
                             0.01f);
    light->moveTo(-5, 1, 1);
    light->radius = 2;
    scene.addLight(light);

    light = new Light(vec3(0, 0, 0),
                      vec3(0.5f, 0, 0),
                      vec3(0.1f, 0, 0),
                      1.f);
    light->moveTo(1, 2, 0);
    light->radius = 1;
    scene.addLight(light);

    buffer = new VBuffer<vec3>;

    qprogram = createShader("lightsphere", VAS_POS3);
    if (qprogram)
        pipeline.watchProgram(qprogram);

}

void Lighting::renderLight(Light* light)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    vec3 dx = vec3(light->radius, 0, 0);
    vec3 dy = vec3(0, light->radius, 0);
    vec3 lightPos = camera.worldToLocal(light->getPosition());

    buffer->clear();
    buffer->pushVertex(lightPos - dx + dy);
    buffer->pushVertex(lightPos - dx - dy);
    buffer->pushVertex(lightPos + dx - dy);
    buffer->pushVertex(lightPos + dx + dy);
    buffer->addQuadI(0);

    qprogram->use();
    light->updateUniforms(camera.getTransform(), *qprogram);

    glDepthMask(false);
    buffer->render();
    glDepthMask(true);
}

void Lighting::render()
{
    //GLApp::render();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(true);
    program->use();
    scene.render(camera, *program, true);
    for (Light* light : scene.lights)
        renderLight(light);
}
