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
#include "scene/rscene.h"
#include "assets.h"
#include "assetmanager.h"

using namespace rgl;
using namespace glm;

vec3 hsvToRGB(float h, float s, float v) {
    float r = 0, g = 0, b = 0;
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
    model = new Model(mesh);
    model->scale = vec3(300, 300, 300);
    model->translate(0, 100, 0);
    model->rotate(-R_PI/2, vec3(0, 1, 0));
    material = PMaterial(vec3(0.1f, 0.1f, 0.6f),
                         vec3(0, 0, 0),
                         0.95f,
                         0.4f);
    model->setMaterial(material);
    model->attachProgram(ShaderPrograms::gpmaterial);
    scene.addModel(model);

    skybox = new Skybox(createCubeMapD(assets::CUBEMAP_DIR + "sea/"));
    pipeline.watchProgram(*ShaderPrograms::gpmaterial);
    pipeline.watchProgram(skybox->getProgram());
    pipeline.watchProgram(*ShaderPrograms::lightPass);
    pipeline.watchProgram(*ShaderPrograms::lightSphere);
}

void Gamma::initGBuffer()
{
    gbuffer = new FrameBuffer(width, height);
    // emission + light accumulation
    gbuffer->addComponent(FrameBuffer::COLOR0, GL_RGBA8);
    // albedo
    gbuffer->addComponent(FrameBuffer::COLOR1, GL_RGBA8);
    // normal + metallic + 2bits of something :p
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
                             0.01f, 0.05f, 0.05f, 1);
    directionalLight.setDirectional(settings.lightDirection);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> floatDistribution(0, 1);
    auto randFloat = bind(floatDistribution, generator);

    Light* light;

    settings.lightAttenuation = vec3(1, 0.001f, 0.001f);
    settings.lightRadius = 150;

    vec3 rgb = vec3(1, 1, 1);
    light = new Light(rgb, 0, 1, 1, vec3(1, 0, 0.0001f));
    light->radius = 400;
    light->moveTo(0, 400, 20);

    scene.addLight(light);
    for (uint i = 1; i < settings.maxLights; i++) {
        rgb = hsvToRGB(randFloat(), 1, 1);
        light = new Light(rgb,
                          0, 1, 1, settings.lightAttenuation);
        light->radius = settings.lightRadius;
        float x = (2 * randFloat() - 1) * 700;
        float y = 20 + randFloat() * 200;
        float z = (2 * randFloat() - 1) * 700;
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
    TwDefine(" Terrain size='240 640' ");

    // display settings
    TwAddVarRW(tweakBar, "Show Depth", TW_TYPE_BOOLCPP, &settings.showDepth, "");
    TwAddVarRW(tweakBar, "Show Normals", TW_TYPE_BOOLCPP, &settings.showNormals, "");
    TwAddVarRW(tweakBar, "Show Albedo", TW_TYPE_BOOLCPP, &settings.showAlbedo, "");

    // light settings
    TwAddVarRW(tweakBar, "Ambient Light", TW_TYPE_BOOLCPP,
               &settings.ambientLighting, " ");
    TwAddVarRW(tweakBar, "Diffuse Light", TW_TYPE_BOOLCPP,
               &settings.diffuseLighting, " ");
    TwAddVarRW(tweakBar, "Specular Light", TW_TYPE_BOOLCPP,
               &settings.specularLighting, " ");

    TwAddVarRW(tweakBar, "Directional Light", TW_TYPE_BOOLCPP,
               &settings.directionalLight, " ");
    TwAddVarRW(tweakBar, "Directional Light Direction", TW_TYPE_DIR3F,
               &directionalLight.direction, " ");
    TwAddVarRW(tweakBar, "Directional Light Color", TW_TYPE_COLOR3F,
               &directionalLight.color, " ");
    TwAddVarRW(tweakBar, "Directional Light Ambience", TW_TYPE_FLOAT,
               &directionalLight.intensity.x, " min=0 max=1 step=0.01 ");
    TwAddVarRW(tweakBar, "Directional Light Diffuse", TW_TYPE_FLOAT,
               &directionalLight.intensity.y, " min=0 max=1 step=0.01 ");
    TwAddVarRW(tweakBar, "Directional Light Specular", TW_TYPE_FLOAT,
               &directionalLight.intensity.z, " min=0 max=1 step=0.01 ");

    TwAddVarRW(tweakBar, "Point Light", TW_TYPE_BOOLCPP,
               &settings.pointLights, " ");
    TwAddVarRW(tweakBar, "Point Lights", TW_TYPE_UINT32,
               &settings.numLights, " min = 0 max = 1024 ");
    TwAddVarRW(tweakBar, "Radius", TW_TYPE_FLOAT,
               &settings.lightRadius, " min=0 max=500 step=1 ");
    TwAddVarRW(tweakBar, "Attenuation", TW_TYPE_BOOLCPP,
                   &settings.attenuation, " ");
    TwAddVarRW(tweakBar, "Attenuation.b", TW_TYPE_FLOAT,
               &settings.lightAttenuation.y, " min=0 max=1 step=0.0001 ");
    TwAddVarRW(tweakBar, "Attenuation.c", TW_TYPE_FLOAT,
               &settings.lightAttenuation.z, " min=0 max=1 step=0.0001 ");

    // material settings
    TwAddVarRW(tweakBar, "mat.albedo", TW_TYPE_COLOR3F, &material.albedo, " ");
    TwAddVarRW(tweakBar, "mat.emission", TW_TYPE_COLOR3F, &material.emission, " ");
    TwAddVarRW(tweakBar, "mat.metallic", TW_TYPE_FLOAT, &material.metallic, " min=0 max=1 step=0.001 ");
    TwAddVarRW(tweakBar, "mat.roughness", TW_TYPE_FLOAT, &material.roughness, " min=0 max=1 step=0.001 ");

    // rendering settings
    TwAddVarRW(tweakBar, "Render Skybox", TW_TYPE_BOOLCPP, &settings.renderSkybox, "");
    TwAddVarRW(tweakBar, "Render Terrain", TW_TYPE_BOOLCPP, &settings.renderTerrain, "");
    TwAddVarRW(tweakBar, "Render Model", TW_TYPE_BOOLCPP, &settings.renderModel, "");
    TwAddVarRW(tweakBar, "Render Wireframe", TW_TYPE_BOOLCPP, &settings.wireframe, "");
    TwAddVarRW(tweakBar, "Render lightspheres", TW_TYPE_BOOLCPP, &settings.renderLights, "");
    TwAddVarRW(tweakBar, "Gamma correction", TW_TYPE_BOOLCPP, &settings.gamma, "");
    // toggle tweakbar on
    toggles[0] = true;
}

void Gamma::prerender()
{
    model->setMaterial(material);

    settings.flags = ShaderProgram::BASE;
    if (settings.ambientLighting)
        settings.flags |= ShaderProgram::AMBIENT;
    if (settings.diffuseLighting)
        settings.flags |= ShaderProgram::DIFFUSE;
    if (settings.specularLighting)
        settings.flags |= ShaderProgram::SPECULAR;
    if (settings.gamma)
        settings.flags |= ShaderProgram::GAMMA;
    if (settings.attenuation)
        settings.flags |= ShaderProgram::ATTENUATION;

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

    if (settings.renderModel)
        scene.render();
    if (settings.renderTerrain)
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
    ShaderPrograms::lightPass->setUniform1i("u_flags", settings.flags);
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
        else if (i == 0 || (light->type == Light::DIRECTIONAL
                            && settings.directionalLight)) {
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
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    ShaderPrograms::UI->setUniform1i("u_flags", settings.flags);
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
    if (TwEventMouseButtonGLFW(button, action))
        return;
    GLApp::mouseButtonCallback(window, button, action, mods);
}

void Gamma::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (TwEventMouseWheelGLFW(yoffset))
        return;
    GLApp::scrollCallback(window, width, height);
    camera.translateLocally(0, 0, -yoffset * 0.1 * _settings.cameraSpeed);
}

void Gamma::resizeCallback(GLFWwindow* window, int width, int height)
{
    GLApp::resizeCallback(window, width, height);
    TwWindowSize(width, height);
}

int main(int argc, char *argv[])
{
    GLApp* app = new Gamma;
    app->init(640, 480, "Deferred Shading Test");
    app->run();
    return 0;
}
