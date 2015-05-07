/**
 * @file light.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */

#include "math/movable3.h"
#include "math/tr3.h"

#include "../gl/shaderprogram.h"

#include "light.h"

void Light::updateCommonUniforms(const std::string& prefix,
                                 rgl::ShaderProgram& program) const
{
    if (program.hasUniform(prefix + UL_TYPE))
        program.setUniform1i(prefix + UL_TYPE, type);

    if (program.hasUniform(prefix + UL_AMBIENT))
        program.setUniform3f(prefix + UL_AMBIENT, ambient);

    if (program.hasUniform(prefix + UL_DIFFUSE))
        program.setUniform3f(prefix + UL_DIFFUSE, diffuse);

    if (program.hasUniform(prefix + UL_SPECULAR))
        program.setUniform3f(prefix + UL_SPECULAR, specular);

    if (program.hasUniform(prefix + UL_ATTENUATION))
        program.setUniform3f(prefix + UL_ATTENUATION, attenuation);

    if (program.hasUniform(prefix + UL_CONEANGLE))
        program.setUniform1f(prefix + UL_CONEANGLE, coneAngle);
}

void Light::updateUniforms(rgl::ShaderProgram& program, int i) const
{
    std::string prefix = i == -1 ? "light." : "light[" + std::to_string(i) + "].";


    updateCommonUniforms(prefix, program);

    if (type == POINT || type == SPOT) {
        if (program.hasUniform(prefix + UL_POSITION))
            program.setUniform3f(prefix + UL_POSITION,
                                 getPosition());
    }

    if (type == DIRECTIONAL || type == SPOT) {
        if (program.hasUniform(prefix + UL_DIRECTION))
            program.setUniform3f(prefix + UL_DIRECTION,
                                 localToWorld(direction));
    }

}

void Light::updateUniforms(const rm::Tr3& tX,
                           rgl::ShaderProgram& program, int i) const
{
    std::string prefix = i == -1 ? "light." : "light[" + std::to_string(i) + "].";

    updateCommonUniforms(prefix, program);

    if (type == POINT || type == SPOT) {
        if (program.hasUniform(prefix + UL_POSITION))
            program.setUniform3f(prefix + UL_POSITION,
                                 tX.multI(getPosition()));
    }

    if (type == DIRECTIONAL || type == SPOT) {
        if (program.hasUniform(prefix + UL_DIRECTION))
            program.setUniform3f(prefix + UL_DIRECTION,
                                 tX.multIV(localToWorldV(direction)));
    }

}
