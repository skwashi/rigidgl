/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include <stdlib.h>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "gl/glutils.h"
#include "gl/mesh.h"
#include "camera.h"
#include "scene/node.h"
#include "cubes.h"
#include "glapp.h"
#include "assets.h"

using namespace std;
using namespace glm;
using namespace rgl;

void Cubes::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);

    camera.init(45.0f, width/ (float) height, 0.1f, 2000.0f);
    camera.moveTo(0, 0, 150);

    // temp stuff for testing

    cubeMesh = std::make_shared<Mesh>();

    cubeMesh->addVertex(vec3(-0.5f, 0.5f, 0.5f));
    cubeMesh->addVertex(vec3(-0.5f, -0.5f, 0.5f));
    cubeMesh->addVertex(vec3(0.5f, -0.5f, 0.5f));
    cubeMesh->addVertex(vec3(0.5f, 0.5f, 0.5f));
    cubeMesh->addVertex(vec3(-0.5f, 0.5f, -0.5f));
    cubeMesh->addVertex(vec3(-0.5f, -0.5f, -0.5f));
    cubeMesh->addVertex(vec3(0.5f, -0.5f, -0.5f));
    cubeMesh->addVertex(vec3(0.5f, 0.5f, -0.5f));

    cubeMesh->addColor(vec4(-0.5f, 0.5f, 0.5f, 1.f));
    cubeMesh->addColor(vec4(-0.5f, -0.5f, 0.5f, 1.f));
    cubeMesh->addColor(vec4(0.5f, -0.5f, 0.5f, 1.f));
    cubeMesh->addColor(vec4(0.5f, 0.5f, 0.5f, 1.f));
    cubeMesh->addColor(vec4(-0.5f, 0.5f, -0.5f, 1.f));
    cubeMesh->addColor(vec4(-0.5f, -0.5f, -0.5f, 1.f));
    cubeMesh->addColor(vec4(0.5f, -0.5f, -0.5f, 1.f));
    cubeMesh->addColor(vec4(0.5f, 0.5f, -0.5f, 1.f));

    int faceF[] = {0, 1, 2, 3};
    int faceB[] = {7, 6, 5, 4};
    int faceL[] = {4, 5, 1, 0};
    int faceR[] = {3, 2, 6, 7};
    int faceU[] = {4, 0, 3, 7};
    int faceD[] = {1, 5, 6, 2};

    cubeMesh->addFace(Face(4, faceF));
    cubeMesh->addFace(Face(4, faceB));
    cubeMesh->addFace(Face(4, faceL));
    cubeMesh->addFace(Face(4, faceR));
    cubeMesh->addFace(Face(4, faceU));
    cubeMesh->addFace(Face(4, faceD));

    cubeMesh->computeFaceNormals();
    cubeMesh->computeVertexNormals();
    cubeMesh->useColors(true);

    cubeMesh->bufferData(false, GL_STATIC_DRAW);

    Node* pivot = sceneGraph.allocateNode();
    pivot->w = vec3(0, 0.5f, 0);

    Model* model = new Model(sceneGraph.allocateNode(), cubeMesh);
    sceneGraph.attachNode(model->node, pivot);
    model->scale = vec3(20, 20, 20);
    model->translate(50, 0, 0);
    model->node->w = vec3(-R_PI/2, 0, 0);
    scene.addModel(model);

    model = new Model(sceneGraph.allocateNode(), cubeMesh);
    sceneGraph.attachNode(model->node, pivot);
    model->scale = vec3(20, 20, 20);
    model->translate(-50, 0, 0);
    model->node->w = vec3(0, R_PI/2, 0);
    scene.addModel(model);

    Node* boxNode = sceneGraph.allocateNode();
    boxNode->w = vec3(R_PI/8, 0, 0);

    int w = 100, h = 20;
    for (int i = -w / 2; i <= w / 2; i++)
        for (int j = -h / 2; j <= h / 2; j++) {
            model = new Model(sceneGraph.allocateNode(), cubeMesh);
            model->scale = vec3(2, 2, 2);
            model->moveTo(i * 9, j * 9, 0);
            sceneGraph.attachNode(model->node, boxNode);
            model->node->w = vec3(0, R_PI/2, 0);
            scene.addModel(model);
        }

    string dir = assets::SHADER_DIR;
    string vertSrc = readFile((dir + "/normcol.vert").c_str());
    string fragSrc = readFile((dir + "/normcol.frag").c_str());

    program.create(vertSrc, fragSrc, VAS_POSNORMCOL);
    pipeline.watchProgram(program);
}

void Cubes::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.use();
    scene.render(program);
    program.disable();
}


void Cubes::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLApp::keyCallback(window, key, scancode, action, mods);
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        cubeMesh->bufferData(!cubeMesh->flatShading, GL_STATIC_DRAW);
    }
}
