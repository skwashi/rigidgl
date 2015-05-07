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

using namespace std;
using namespace rgl;
using namespace rutils;
using glm::vec3;

void Lighting::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);
    camera.init(45.0f, width/ (float) height, 0.1f, 2000.0f);
    camera.moveTo(0, 0, 5);

    ShaderProgram* cprogram = createShader("normcol", VAS_POSNORMCOL);
    if (cprogram)
        pipeline.watchProgram(*cprogram);

    program = createShader("mesh", VAS_POSNORM);
    if (program)
        pipeline.watchProgram(program);

    Mesh* mesh = new Mesh();
    bool success = loadObj(assets::MODEL_DIR + "teapot.obj", *mesh);

    if (success) {
        mesh->bufferData();
        Model* model = new Model(sceneGraph.allocateNode(), mesh);
        model->node->w = vec3(0, -R_PI/2, 0);
        model->attachProgram(cprogram);
        scene.addModel(model);
    }

    Light* light = new Light(vec3(0.005f, 0.01f, 0.005f),
                             vec3(0.2f, 0.2f, 0.5f),
                             vec3(0.3f, 0.3f, 0.3f),
                             0.01f);
    light->moveTo(-5, 1, 1);

    scene.addLight(light);
}

void Lighting::render()
{
    //GLApp::render();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program->use();
    scene.render(camera, *program, true);
}
