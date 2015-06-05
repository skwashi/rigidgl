/**
 * @file shaderprograms.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-12
 */

#include <vector>
#include <string>
#include "assets.h"
#include "shaderprogram.h"
#include "vertexattrib.h"
#include "shaderprograms.h"
#include "../rutils.h"

namespace rgl {
namespace ShaderPrograms {

const std::string version130 = "#version 130\n";

ShaderProgram* lightSphere = NULL;
ShaderProgram* mesh = NULL;
ShaderProgram* cubeMap = NULL;
ShaderProgram* reflect = NULL;
ShaderProgram* reflectL = NULL;
ShaderProgram* terrain = NULL;
ShaderProgram* terrainL = NULL;
ShaderProgram* depth = NULL;
ShaderProgram* depthUI = NULL;
ShaderProgram* normalUI = NULL;
ShaderProgram* positionUI = NULL;
ShaderProgram* UI = NULL;
ShaderProgram* gpmaterial = NULL;
ShaderProgram* gpterrain = NULL;
ShaderProgram* lightPass = NULL;

ShaderProgram* createShader(const ShaderDef& shaderDef)
{
    return createShader(shaderDef.vertFiles, shaderDef.fragFiles,
                        shaderDef.attribs);
}

ShaderProgram* createShader(const std::string& shaderName,
                            const std::vector<VertexAttrib>& attribs)
{
    return createShader(shaderName + ".vert", shaderName + ".frag",
                        attribs);
}

ShaderProgram* createShader(const std::string& vertName,
                            const std::string& fragName,
                            const std::vector<VertexAttrib>& attribs)
{
    return createShader(&vertName, 1, &fragName, 1, attribs);
}

ShaderProgram* createShader(const std::string* vertFiles, uint vfcount,
                            const std::string* fragFiles, uint ffcount,
                            const std::vector<rgl::VertexAttrib>& attribs)
{
    std::string vertSrc;
    std::string fragSrc;

    for (uint i = 0; i < vfcount; i++) {
        vertSrc += rutils::readFile(assets::SHADER_DIR + vertFiles[i]);
    }

    for (uint i = 0; i < ffcount; i++) {
        fragSrc += rutils::readFile(assets::SHADER_DIR + fragFiles[i]);
    }

    ShaderProgram* program = new ShaderProgram;
    bool success = program->create(vertSrc, fragSrc, attribs);

    if (success) {
        return program;
    }
    else {
        std::cout << "Failed creating ShaderProgram "
                  << vertFiles[vfcount - 1] << " - " << fragFiles[ffcount - 1] << std::endl;
        delete program;
        return NULL;
    }
}

rgl::ShaderProgram* createShader(const std::vector<std::string>& vertFiles,
                                 const std::vector<std::string>& fragFiles,
                                 const std::vector<rgl::VertexAttrib>& attribs)
{
    return createShader(&vertFiles[0], vertFiles.size(),
                        &fragFiles[0], fragFiles.size(),
                        attribs);
}


bool initPrograms()
{
    lightSphere = createShader("lightsphere", VAS_POS3);
    if (lightSphere == NULL)
        return false;

    mesh = createShader("mesh", VAS_POSNORM);
    if (mesh == NULL)
        return false;

    cubeMap = createShader("cubemap", VAS_POSNORM);
    if (cubeMap == NULL)
        return false;

    reflect = createShader("reflect", VAS_POSNORM);
    if (reflect == NULL)
        return false;

    reflectL = createShader("reflectl", VAS_POSNORM);
    if (reflectL == NULL)
        return false;

    terrain = createShader("terrain", VAS_3NT);
    if (terrain == NULL)
        return false;

    terrainL = createShader("terrainl", VAS_3NT);
    if (terrainL == NULL)
        return false;

    depth = createShader("depth", VAS_3T);
    if (depth == NULL)
        return false;

    depthUI = createShader("ui.vert", "depth.frag", VAS_3T);
    if (depthUI == NULL)
        return false;

    normalUI = createShader("ui.vert", "normal.frag", VAS_3T);
    if (normalUI == NULL)
        return false;

    positionUI = createShader("ui.vert", "position.frag", VAS_3T);
    if (positionUI == NULL)
        return false;

    UI = createShader("ui.vert", "t.frag", VAS_3T);
    if (UI == NULL)
        return false;

    gpmaterial = createShader("gpmaterial", VAS_3NT);
    if (gpmaterial == NULL)
        return false;

    gpterrain = createShader("gpterrain", VAS_3NT);
    if (gpterrain == NULL)
        return false;

    lightPass = createShader("lightpass.vert", "lightpass.frag", VAS_3);
    return true;
}

} // namespace
} // namespace
