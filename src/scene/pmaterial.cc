/**
 * @file pmaterial.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-12
 */



#include <string>
#include <glm/glm.hpp>

#include "pmaterial.h"
#include "../gl/shaderprogram.h"

const std::string UM_ALBEDO = "mat.albedo";
const std::string UM_EMISSION = "mat.emission";
const std::string UM_METALLIC = "mat.metallic";
const std::string UM_ROUGHNESS = "mat.roughness";

const PMaterial DEFAULT_PMATERIAL = PMaterial(
    glm::vec3(0.6, 0.6, 0.6),
    glm::vec3(0, 0, 0),
    0.04f,
    0.95f);

void PMaterial::updateUniforms(rgl::ShaderProgram& program) const
{
    if (program.hasUniform(UM_ALBEDO))
        program.setUniform3f(UM_ALBEDO, albedo);

    if (program.hasUniform(UM_EMISSION))
        program.setUniform3f(UM_EMISSION, emission);

    if (program.hasUniform(UM_METALLIC))
        program.setUniform1f(UM_METALLIC, metallic);

    if (program.hasUniform(UM_ROUGHNESS))
        program.setUniform1f(UM_ROUGHNESS, roughness);
}
