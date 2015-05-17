/**
 * @file terraintest.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-16
 */

#include <glm/glm.hpp>

#include "terraintest.h"
#include "glapp.h"
#include "gl/vertexattrib.h"
#include "gl/shaderprogram.h"
#include "gl/shaderprograms.h"
#include "gl/textureutils.h"
#include "gl/cubemap.h"
#include "scene/skybox.h"
#include "assets.h"

using namespace rgl;

void TerrainTest::init(int width, int height, const char* title)
{
    GLApp::init(width, height, title);
    camera.init(45.f * DEG_TO_RAD, width/ (float) height, 0.01f, 1000.0f);
    camera.moveTo(0, 0, 4);
    _settings.cameraSpeed = 100;

    terrain = new Terrain(800);
    terrain->modelMatrix = glm::scale(terrain->modelMatrix, glm::vec3(2, 100, -2));
    GLTexture* tex = createTexture(assets::TEXTURE_DIR + "grad.png");
    tex->setWrap(GL_CLAMP_TO_EDGE);
    terrain->texture = tex;
    scene.addRenderable(terrain);

    skybox = new Skybox(createCubeMapD(assets::CUBEMAP_DIR + "sea/"));
    ShaderProgram* program = ShaderPrograms::createShader("terrain", VAS_3N);
    terrain->attachProgram(program);
    pipeline.watchProgram(program);
    pipeline.watchProgram(skybox->getProgram());
}

void TerrainTest::render()
{
    glDepthMask(GL_FALSE);
    skybox->render();
    glDepthMask(GL_TRUE);
    scene.render();
}
