/**
 * @file terraintest.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-16
 */

#include <glm/glm.hpp>
#include <random>
#include <AntTweakBar.h>

#include "gamma.h"
#include "rutils.h"
#include "gl/rgl.h"
#include "scene/skybox.h"
#include "scene/model.h"
#include "assets.h"
#include "assetmanager.h"

using namespace rgl;
using namespace glm;

vec3 hsvToRGB(float h, float s, float v) {
    float r, g, b;
    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch(i % 6) {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
    }

    return vec3(r, g, b);
}

void Gamma::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);
    camera.init(45.f * DEG_TO_RAD, width/ (float) height, 0.1f, 5000.0f);
    camera.moveTo(-700, 200, 100);
    camera.rotate(-R_TAU/4.f, 0, 1, 0);
    _settings.cameraSpeed = 100;

    initGBuffer();
    initTerrain();
    initLights();
    initTweakBar();

    Mesh* mesh = new MeshN();
    rutils::loadObj(assets::MODEL_DIR + "armadillo.obj", *mesh);
    mesh->bufferData();
    model = Model(mesh);
    model.scale = vec3(300, 300, 300);
    model.translate(0, 100, 0);
    model.rotate(-R_PI/2, vec3(0, 1, 0));
    material = Material(vec3(0.01f, 0.01f, 0.1f),
                        vec3(0.5f, 0.4f, 0.8f),
                        vec3(0.3f, 0.3f, 0.3f),
                        256);
    model.setMaterial(material);
    model.attachProgram(ShaderPrograms::gpmaterial);
    scene.addModel(&model);

    skybox = new Skybox(createCubeMapD(assets::CUBEMAP_DIR + "sea/"));
    pipeline.watchProgram(*ShaderPrograms::gpmaterial);
    pipeline.watchProgram(skybox->getProgram());
    pipeline.watchProgram(*ShaderPrograms::lightPass);
    pipeline.watchProgram(*ShaderPrograms::lightSphere);
}

void Gamma::initGBuffer()
{
    gbuffer = new FrameBuffer(width, height);
    // light accumulation
    gbuffer->addComponent(FrameBuffer::COLOR0, GL_RGBA8);
    // albedo
    gbuffer->addComponent(FrameBuffer::COLOR1, GL_RGBA8);
    // normal + metallicness + 2bits of something :p
    gbuffer->addComponent(FrameBuffer::COLOR2, GL_RGB10_A2, GL_UNSIGNED_INT_10_10_10_2);
    // z
    gbuffer->addComponent(FrameBuffer::COLOR3, GL_R32F, GL_FLOAT);
    // depth + stencil
    gbuffer->addComponent(FrameBuffer::DEPTH_STENCIL, GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8);

    // some quads for buffer display
    float h = 0.5f;
    float w = 0.5f;

    vbuffer3t->pushQuad({{1-w, 1-h, 0}, {0, 0}}, {{1, 1-h, 0}, {1, 0}},
                        {{1, 1, 0}, {1, 1}}, {{1-w, 1, 0}, {0, 1}});

    vbuffer3t->pushQuad({{1-w, 1-2*h, 0}, {0, 0}}, {{1, 1-2*h, 0}, {1, 0}},
                        {{1, 1-h, 0}, {1, 1}}, {{1-w, 1-h, 0}, {0, 1}});

    vbuffer3t->pushQuad({{1-w, 1-3*h, 0}, {0, 0}}, {{1, 1-3*h, 0}, {1, 0}},
                        {{1, 1-2*h, 0}, {1, 1}}, {{1-w, 1-2*h, 0}, {0, 1}});

    vbuffer3t->pushQuad({{1-w, 1-4*h, 0}, {0, 0}}, {{1, 1-4*h, 0}, {1, 0}},
                        {{1, 1-3*h, 0}, {1, 1}}, {{1-w, 1-3*h, 0}, {0, 1}});

    vbuffer3t->pushQuad({{-1, -1, 0}, {0, 0}}, {{1, -1, 0}, {1, 0}},
                        {{1, 1, 0}, {1, 1}}, {{-1, 1, 0}, {0, 1}});

}

void Gamma::initTerrain()
{
    terrain = new Terrain(400);
    terrain->modelMatrix = glm::translate(glm::scale(terrain->modelMatrix, glm::vec3(4, 100, -4)), glm::vec3(-200, 0, -200));

    terrain->updateMatrices();

    GLTexture* texture = createTexture(assets::TEXTURE_DIR + "grad.png");
    texture->setWrap(GL_CLAMP_TO_EDGE);
    terrain->texture = texture;
    terrain->attachProgram(ShaderPrograms::gpterrain);
    pipeline.watchProgram(*ShaderPrograms::gpterrain);
}

void Gamma::initLights()
{
    Node* axis = sceneGraph.allocateNode();
    axis->w = vec3(0, R_PI/16, 0);

    settings.lightDirection = normalize(vec3(1, -10, 0));
    directionalLight = Light(vec3(0.8f, 0.8f, 0.4f),
                             0.01, 0.05, 0.05, 1);
    directionalLight.setDirectional(settings.lightDirection);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> floatDistribution(0, 1);
    auto randFloat = bind(floatDistribution, generator);

    Light* light;

    settings.lightAttenuation = vec3(1, 0.005f, 0.005f);
    settings.lightRadius = 150;

    for (uint i = 0; i < settings.maxLights; i++) {
        vec3 rgb = hsvToRGB(randFloat(), 1, 1);
        light = new Light(rgb,
                          0, 1, 1, settings.lightAttenuation);
        light->radius = settings.lightRadius;
        float x = (2 * randFloat() - 1) * 700; //400
        float y = 20 + randFloat() * 200;
        float z = (2 * randFloat() - 1) * 700; //400
        light->moveTo(x, y, z);
        light->attachNode(sceneGraph.allocateNode());
        sceneGraph.attachNode(light->node, axis);
        scene.addLight(light);
    }

}

void Gamma::initTweakBar()
{
    TwInit(TW_OPENGL, NULL);
    TwWindowSize(width, height);
    TwBar* tweakBar = TwNewBar("Terrain");
    TwDefine(" Terrain size='240 320' ");

    // display settings
    TwAddVarRW(tweakBar, "Show Depth", TW_TYPE_BOOLCPP, &settings.showDepth, "");
    TwAddVarRW(tweakBar, "Show Normals", TW_TYPE_BOOLCPP, &settings.showNormals, "");
    TwAddVarRW(tweakBar, "Show Albedo", TW_TYPE_BOOLCPP, &settings.showAlbedo, "");

    // light settings
    TwAddVarRW(tweakBar, "Directional Light", TW_TYPE_BOOLCPP,
               &settings.directionalLight, " ");
    TwAddVarRW(tweakBar, "Directional Light Direction", TW_TYPE_DIR3F,
               &directionalLight.direction, " ");
    TwAddVarRW(tweakBar, "Directional Light Diffuse", TW_TYPE_COLOR3F,
               &directionalLight.diffuse, " ");
    TwAddVarRW(tweakBar, "Directional Light Specular", TW_TYPE_COLOR3F,
               &directionalLight.specular, " ");

    TwAddVarRW(tweakBar, "Point Light", TW_TYPE_BOOLCPP,
               &settings.pointLights, " ");
    TwAddVarRW(tweakBar, "Point Lights", TW_TYPE_UINT32,
               &settings.numLights, " min = 0 max = 1024 ");
    TwAddVarRW(tweakBar, "Point Light Radius", TW_TYPE_FLOAT,
               &settings.lightRadius, " min=0 max=500 step=1 ");
    TwAddVarRW(tweakBar, "Point Light Attenuation.b", TW_TYPE_FLOAT,
               &settings.lightAttenuation.y, " min=0 max=1 step=0.0001 ");
    TwAddVarRW(tweakBar, "Point Light Attenuation.c", TW_TYPE_FLOAT,
               &settings.lightAttenuation.z, " min=0 max=1 step=0.0001 ");

    // material settings
    TwAddVarRW(tweakBar, "mat.ambient", TW_TYPE_COLOR3F, &material.ambient, " colormode = hls ");
    TwAddVarRW(tweakBar, "mat.diffuse", TW_TYPE_COLOR3F, &material.diffuse, " colormode = hls ");
    TwAddVarRW(tweakBar, "mat.specular", TW_TYPE_COLOR3F, &material.specular, " colormode = hls ");
    TwAddVarRW(tweakBar, "mat.shininess", TW_TYPE_FLOAT, &material.shininess, " min=0 max=2048 step=0.125 ");

    // rendering settings
    TwAddVarRW(tweakBar, "Render Skybox", TW_TYPE_BOOLCPP, &settings.renderSkybox, "");
    TwAddVarRW(tweakBar, "Render lightspheres", TW_TYPE_BOOLCPP, &settings.renderLights, "");

    // toggle tweakbar on
    toggles[0] = true;
}

void Gamma::prerender()
{
    model.setMaterial(material);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Gamma::render()
{
    renderGeometry();
    renderLighting();
    renderDisplay();
}

void Gamma::renderGeometry()
{
    gbuffer->bind();
    if (settings.wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ShaderPrograms::gpmaterial->use();

    scene.render();
    terrain->render();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    gbuffer->unbind();
}

void Gamma::renderLighting()
{
    gbuffer->draw(FrameBuffer::COLOR0);
    gbuffer->bindTexture(0, FrameBuffer::COLOR1);
    gbuffer->bindTexture(1, FrameBuffer::COLOR2);
    gbuffer->bindTexture(2, FrameBuffer::COLOR3);
    glActiveTexture(GL_TEXTURE0);

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    ShaderPrograms::lightPass->use();
    ShaderPrograms::lightPass->setUniform1f("xRatio", camera.getXRatio());
    ShaderPrograms::lightPass->setUniform1f("yRatio", camera.getYRatio());
    ShaderPrograms::lightPass->setUniform1i("albedoMap", 0);
    ShaderPrograms::lightPass->setUniform1i("normalMap", 1);
    ShaderPrograms::lightPass->setUniform1i("depthMap", 2);
    ShaderPrograms::lightPass->setUniform2i("u_screenSize", ivec2(width, height));

    if (settings.directionalLight) {
        directionalLight.updateUniforms(camera.getTransform(),
                                         *ShaderPrograms::lightPass);
        rgl::getQuadBuffer()->render();
    }

    for (uint i = 0; i < settings.numLights; i++) {
        Light* light = scene.lights[i];
        if (!light->isEnabled())
            continue;
        else if (light->type == Light::DIRECTIONAL
                 && settings.directionalLight) {
            light->updateUniforms(camera.getTransform(), *ShaderPrograms::lightPass);
            rgl::getQuadBuffer()->render();
        }
        else if (light->type == Light::POINT
                 && settings.pointLights) {
            vec3 posl = light->getPosition();
            vec3 pos = light->getPositionW();
            vec3 top = terrain->getTop(pos);
            float h = top.y;
            if (h >= 0 && h >= pos.y) {
                top.y += settings.heightAboveTerrain;
                light->moveToW(top, false);
            }
            light->attenuation = settings.lightAttenuation;
            light->radius = settings.lightRadius;
            light->updateUniforms(camera.getTransform(), *ShaderPrograms::lightPass);
            rgl::getQuadBuffer()->render();
            light->moveTo(posl);
        }
    }

    if (settings.renderSkybox)
        skybox->render();

    if (settings.renderLights) {
        ShaderProgram* program = ShaderPrograms::lightSphere;
        program->use();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        for (uint i = 0; i < settings.numLights; i++) {
            Light* light = scene.lights[i];
            if (light->isEnabled() && light->type == Light::POINT) {
                light->updateUniforms(camera.getTransform(), *program);
                light->render(*program);
            }
        }
    }

    gbuffer->unbind();

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void Gamma::renderDisplay()
{
    ShaderPrograms::UI->use();
    gbuffer->bindTexture(0, FrameBuffer::COLOR0);
    vbuffer3t->renderItem(4);

    if (settings.showDepth) {
        ShaderPrograms::depthUI->use();
        ShaderPrograms::depthUI->setUniform1f("zFar", camera.getFar());
        gbuffer->bindTexture(0, FrameBuffer::COLOR3);
        vbuffer3t->renderItem(0);
    }

    if (settings.showNormals) {
        ShaderPrograms::normalUI->use();
        gbuffer->getTexture(FrameBuffer::COLOR2)->bind();
        vbuffer3t->renderItem(1);
    }

    if (settings.showAlbedo) {
        ShaderPrograms::UI->use();
        gbuffer->getTexture(FrameBuffer::COLOR1)->bind();
        vbuffer3t->renderItem(2);
    }

    if (toggles[0])
        TwDraw();
}

void Gamma::destroy()
{
    TwTerminate();
}

void Gamma::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    GLApp::cursorCallback(window, xpos, ypos);
    TwEventMousePosGLFW(xpos, ypos);
}

void Gamma::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    GLApp::mouseButtonCallback(window, button, action, mods);
    if (TwEventMouseButtonGLFW(button, action))
        return;
}

void Gamma::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    GLApp::scrollCallback(window, width, height);
    if (TwEventMouseWheelGLFW(yoffset))
        return;
}

void Gamma::resizeCallback(GLFWwindow* window, int width, int height)
{
    GLApp::resizeCallback(window, width, height);
    TwWindowSize(width, height);
}
