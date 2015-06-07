/**
 * @file light.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */

#include <glm/glm.hpp>

#include "math/movable3.h"
#include "math/tr3.h"

#include "../gl/shaderprogram.h"
#include "../gl/vertexbuffer.h"
#include "../gl/vertexbuffer.h"
#include "meshes.h"

#include "light.h"

const std::string UL_TYPE = "type";
const std::string UL_POSITION = "position";
const std::string UL_DIRECTION = "direction";
const std::string UL_COLOR = "color";
const std::string UL_INTENSITY = "intensity";
const std::string UL_ATTENUATION = "attenuation";
const std::string UL_CONEANGLE = "coneAngle";
const std::string UL_RADIUS = "radius";

void Light::updateCommonUniforms(const std::string& prefix,
                                 rgl::ShaderProgram& program) const
{
    if (program.hasUniform(prefix + UL_TYPE))
        program.setUniform1i(prefix + UL_TYPE, type);

    if (program.hasUniform(prefix + UL_COLOR))
        program.setUniform3f(prefix + UL_COLOR, color);

    if (program.hasUniform(prefix + UL_INTENSITY))
        program.setUniform3f(prefix + UL_INTENSITY, intensity);

    if (program.hasUniform(prefix + UL_ATTENUATION))
        program.setUniform3f(prefix + UL_ATTENUATION, attenuation);

    if (program.hasUniform(prefix + UL_CONEANGLE))
        program.setUniform1f(prefix + UL_CONEANGLE, coneAngle);

    if (program.hasUniform(prefix + UL_RADIUS))
        program.setUniform1f(prefix + UL_RADIUS, radius);
}

void Light::updateUniforms(rgl::ShaderProgram& program, int i) const
{
    std::string prefix = i == -1 ? "light." : "light[" + std::to_string(i) + "].";

    updateCommonUniforms(prefix, program);

    if (type == POINT || type == SPOT || type == DIRECTIONAL) {
        if (program.hasUniform(prefix + UL_POSITION))
            program.setUniform3f(prefix + UL_POSITION,
                                 getPositionW());
    }

    if (type == DIRECTIONAL || type == SPOT) {
        if (program.hasUniform(prefix + UL_DIRECTION))
            program.setUniform3f(prefix + UL_DIRECTION,
                                 localToWorldVW(direction));
    }

}

void Light::updateUniforms(const rm::Tr3& tX,
                           rgl::ShaderProgram& program, int i) const
{
    std::string prefix = i == -1 ? "light." : "light[" + std::to_string(i) + "].";

    updateCommonUniforms(prefix, program);

    if (type == POINT || type == SPOT || type == DIRECTIONAL) {
        if (program.hasUniform(prefix + UL_POSITION))
            program.setUniform3f(prefix + UL_POSITION,
                                 tX.multI(getPositionW()));
    }

    if (type == DIRECTIONAL || type == SPOT) {
        if (program.hasUniform(prefix + UL_DIRECTION))
            program.setUniform3f(prefix + UL_DIRECTION,
                                 tX.multIV(localToWorldVW(direction)));
    }

}

void Light::render(rgl::ShaderProgram& program) const {
    rgl::getQuadBuffer()->render();
}

