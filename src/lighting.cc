/**
 * @file lighting.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */


#include <GL/glew.h>

#include "lighting.h"
#include "scene/light.h"
#include "assets.h"
#include "rutils.h"
#include "gl/rgl.h"
#include "scene/meshes.h"

using namespace std;
using namespace rgl;
using namespace rutils;
using glm::vec2;
using glm::vec3;
using glm::vec4;


void Lighting::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);
    camera.init(45.0f * DEG_TO_RAD, width/ (float) height, 0.01f, 200.0f);
    camera.moveTo(0, 1, 7);

    ShaderProgram* cprogram = ShaderPrograms::createShader("normcol", VAS_POSNORMCOL);
    if (cprogram)
        pipeline.watchProgram(*cprogram);

    program = ShaderPrograms::createShader("mesh", VAS_POSNORM);
    if (program)
        pipeline.watchProgram(program);

    Mesh* mesh = new MeshN();
    bool success = loadObj(assets::MODEL_DIR + "teapot.obj", *mesh);

    if (success) {
        mesh->bufferData();
        Model* model = new Model(sceneGraph.allocateNode(), mesh);
        model->moveTo(-1, 0, 0);
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

    Node* axis = sceneGraph.allocateNode();
    axis->w = vec3(0, R_PI/8, 0);

    Light* light = new Light(vec3(0.0001, 0.0001, 0.0001),
                             vec3(0.1f, 0.1f, 0.5f),
                             vec3(0.1f, 0.1f, 0.1f),
                             0.01f);

    light->radius = 2;
    light->attachNode(sceneGraph.allocateNode());
    sceneGraph.attachNode(light->node, axis);
    light->moveTo(-5, 1, 1);

    scene.addLight(light);

    light = new Light(vec3(0, 0, 0),
                      vec3(0.5f, 0, 0),
                      vec3(0.1f, 0, 0),
                      1.f);
    light->moveTo(1, 2, 0);
    light->radius = 1;
    scene.addLight(light);

    pipeline.watchProgram(ShaderPrograms::lightSphere);
}

void Lighting::render()
{
    scene.render(camera, *program, true);
    scene.renderLights(camera, *ShaderPrograms::lightSphere);
}
