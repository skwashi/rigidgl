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

ShaderProgram* lightSphere = NULL;
ShaderProgram* mesh = NULL;
ShaderProgram* cubeMap = NULL;
ShaderProgram* reflect = NULL;
ShaderProgram* reflectL = NULL;

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
    std::string vertSrc = rutils::readFile(
        assets::SHADER_DIR + vertName);
    std::string fragSrc = rutils::readFile(
        assets::SHADER_DIR + fragName);

    ShaderProgram* program = new ShaderProgram;
    bool success = program->create(vertSrc, fragSrc, attribs);

    if (success) {
        return program;
    } else {
        std::cout << "Failed creating ShaderProgram "
                  << vertName << " - " << fragName << std::endl;
        delete program;
        return NULL;
    }
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

    return true;
}

} // namespace
} // namespace
