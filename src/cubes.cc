/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include <stdlib.h>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <png.h>
#include <AntTweakBar.h>

#include "rutils.h"
#include "gl/rgl.h"
#include "camera.h"
#include "scene/node.h"
#include "scene/meshes.h"
#include "scene/skybox.h"
#include "cubes.h"
#include "glapp.h"
#include "assets.h"
#include "graphics/batch.h"

// Font test
#include <stdio.h>
#include <wchar.h>


using namespace rutils;
using namespace std;
using namespace glm;
using namespace rgl;

void Cubes::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);

    camera.init(45.0f * DEG_TO_RAD, width/ (float) height, 0.1f, 400.0f);
    camera.moveTo(0, 0, 150);

    ShaderPrograms::depthUI->use();
    ShaderPrograms::depthUI->setUniform1f("zNear", 0.1f);
    ShaderPrograms::depthUI->setUniform1f("zFar", 400.0f);

    pipeline.watchProgram(*ShaderPrograms::gpmaterial);

    // TwInit(TW_OPENGL, NULL);
    // TwWindowSize(width, height);
    // TwBar* bar = TwNewBar("Empty TweakBar");
    // float myVar;
    // TwAddVarRW(bar, "NameOfMyVariable", TW_TYPE_FLOAT, &myVar, "");

    // temp stuff for testing

    cubeMesh = new MeshNTC();

    cubeMesh->addVertex(vec3(-0.5f, 0.5f, 0.5f));
    cubeMesh->addVertex(vec3(-0.5f, -0.5f, 0.5f));
    cubeMesh->addVertex(vec3(0.5f, -0.5f, 0.5f));
    cubeMesh->addVertex(vec3(0.5f, 0.5f, 0.5f));
    cubeMesh->addVertex(vec3(-0.5f, 0.5f, -0.5f));
    cubeMesh->addVertex(vec3(-0.5f, -0.5f, -0.5f));
    cubeMesh->addVertex(vec3(0.5f, -0.5f, -0.5f));
    cubeMesh->addVertex(vec3(0.5f, 0.5f, -0.5f));

    cubeMesh->addTexCoord(TexCoord(0, 1));
    cubeMesh->addTexCoord(TexCoord(0, 0));
    cubeMesh->addTexCoord(TexCoord(1, 0));
    cubeMesh->addTexCoord(TexCoord(1, 1));

    cubeMesh->addColor(Color(1, 0, 0, 1));
    cubeMesh->addColor(Color(0, 1, 0, 1));
    cubeMesh->addColor(Color(0, 0, 1, 1));

    cubeMesh->addFace(Face(0, 1, 2, 3,
                           0, 1, 2, 3,
                           0, 0, 0, 0));
    cubeMesh->addFace(Face(7, 6, 5, 4,
                           0, 1, 2, 3,
                           0, 0, 0, 0));
    cubeMesh->addFace(Face(4, 5, 1, 0,
                           0, 1, 2, 3,
                           1, 1, 1, 1));
    cubeMesh->addFace(Face(3, 2, 6, 7,
                           0, 1, 2, 3,
                           1, 1, 1, 1));
    cubeMesh->addFace(Face(4, 0, 3, 7,
                           0, 1, 2, 3,
                           2, 2, 2, 2));
    cubeMesh->addFace(Face(1, 5, 6, 2,
                           0, 1, 2, 3,
                           2, 2, 2, 2));

    cubeMesh->computeFaceNormals();
    cubeMesh->computeVertexNormals();

    cubeMesh->bufferFaceData(true, GL_STATIC_DRAW);

    // load textures
    string texname = assets::TEXTURE_DIR + "stGrid1.png";

    uint wi, he, bpp;
    byte* data;
    loadImage(texname.c_str(), wi, he, bpp, &data);
    GLint intFormat = bpp == 24 ? GL_RGB : GL_RGBA;
    GLenum format = bpp == 24 ? GL_BGR : GL_BGRA;

    GLTexture* tex = new GLTexture(intFormat, format, wi, he, data);

    gbuffer = FrameBuffer::create(width, height, 5,
                                  FrameBuffer::COLOR0, GL_R32F,
                                  FrameBuffer::COLOR1, GL_R32F,
                                  FrameBuffer::COLOR2, GL_R32F,
                                  FrameBuffer::COLOR3, GL_RG16F,
                                  FrameBuffer::DEPTH, GL_DEPTH_COMPONENT32);


    vbuffer3t->pushQuad({{0.5f, 0.5f, 0}, {0, 0}}, {{1, 0.5f, 0}, {1, 0}},
                        {{1, 1.0f, 0}, {1, 1}}, {{0.5f, 1.0f, 0}, {0, 1}});

    // vbuffer3t->pushQuad({{0.5f, 0.f, 0}, {0, 0}}, {{1, 0.f, 0}, {1, 0}},
    //                     {{1, 0.5f, 0}, {1, 1}}, {{0.5f, 0.5f, 0}, {0, 1}});

    // vbuffer3t->pushQuad({{0.5f, -0.5f, 0}, {0, 0}}, {{1, -0.5f, 0}, {1, 0}},
    //                     {{1, 0.f, 0}, {1, 1}}, {{0.5f, 0.f, 0}, {0, 1}});

    float w = 1, h = 1;
    vbuffer3t->pushQuad({{1-w, 1-h, 0}, {0, 0}}, {{1, 1-h, 0}, {1, 0}},
                        {{1, 1, 0}, {1, 1}}, {{1-w, 1, 0}, {0, 1}});

    vbuffer3t->pushQuad({{1-w, 1-2*h, 0}, {0, 0}}, {{1, 1-2*h, 0}, {1, 0}},
                        {{1, 1-h, 0}, {1, 1}}, {{1-w, 1-h, 0}, {0, 1}});


    string cubemapDir = assets::CUBEMAP_DIR + "RomeChurch/";

    CubeMap* cmap = createCubeMap(
        cubemapDir + "negx.png", cubemapDir + "posx.png",
        cubemapDir + "negy.png", cubemapDir + "posy.png",
        cubemapDir + "negz.png", cubemapDir + "posz.png");

    CubeMap* cm = new CubeMap(intFormat, format, wi,
                              data, data, data, data, data, data, true);

    Mesh* cmesh = createCubeMesh();
    cmesh->bufferData();

    Node* pivot = sceneGraph.allocateNode();
    pivot->w = vec3(0, 0.5f, 0);

    string dir = assets::SHADER_DIR;
    string vertSrc = readFile((dir + "normcol.vert").c_str());
    string fragSrc = readFile((dir + "normcol.frag").c_str());

    cprogram.create(vertSrc, fragSrc, VAS_POSNORMCOL);
    pipeline.watchProgram(cprogram);

    tprogram.create(readFile((dir + "normtex.vert").c_str()),
                    readFile((dir + "normtex.frag").c_str()),
                    VAS_POSNORMTEX);
    pipeline.watchProgram(tprogram);

    skybox = new Skybox(cmap);
    pipeline.watchProgram(skybox->getProgram());

    ShaderProgram* cpr = new ShaderProgram() ;
    cpr->create(readFile((dir + "cubemap.vert").c_str()),
                readFile((dir + "cubemap.frag").c_str()),
                VAS_POSNORM);
    pipeline.watchProgram(*cpr);

    Model* model = new Model(sceneGraph.allocateNode(), cubeMesh, tex);
    sceneGraph.attachNode(model->node, pivot);
    model->scale = vec3(20, 20, 20);
    model->translate(50, 0, 0);
    model->node->w = vec3(-R_PI/2, 0, 0);
    model->attachProgram(&tprogram);
    scene.addModel(model);

    model = new Model(sceneGraph.allocateNode(), cubeMesh, tex);
    sceneGraph.attachNode(model->node, pivot);
    model->scale = vec3(20, 20, 20);
    model->translate(-50, 0, 0);
    model->node->w = vec3(0, R_PI/2, 0);
    model->attachProgram(&tprogram);
    scene.addModel(model);


    Mesh* testMesh = new MeshN();
    bool success = loadObj((assets::MODEL_DIR + "bunny.obj").c_str(), *testMesh);
    if (success) {
        testMesh->bufferData();
        model = new Model(sceneGraph.allocateNode(), testMesh);
        model->scale = vec3(20, 20, 20);
        model->attachProgram(&cprogram);
        scene.addModel(model);
    }

    Node* boxNode = sceneGraph.allocateNode();
    boxNode->w = vec3(R_PI/8, 0, 0);

    int wid = 100, hei = 20;
    for (int i = -wid / 2; i <= wid / 2; i++)
        for (int j = -hei / 2; j <= hei / 2; j++) {
            model = new Model(sceneGraph.allocateNode(), cmesh, cm);
            model->scale = vec3(1, 1, 1);
            model->moveTo(i * 9, j * 9, 0);
            sceneGraph.attachNode(model->node, boxNode);
            model->node->w = vec3(0, R_PI/2, 0);
            model->attachProgram(cpr);
            scene.addModel(model);
        }
    program = &cprogram;
}

void Cubes::render()
{
    scene.render();
}


void Cubes::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLApp::keyCallback(window, key, scancode, action, mods);

    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        scene.models[3]->getTexture()->bind();
        if (mipmap) {
            scene.models[3]->getTexture()->setFilter(GL_LINEAR, GL_LINEAR);

        } else {
            scene.models[3]->getTexture()->setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        }
        mipmap = !mipmap;
        if (program == &tprogram)
            program = &cprogram;
        else
            program = &tprogram;
    }

}

