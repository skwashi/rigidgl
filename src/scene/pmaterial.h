/**
 * @file pmaterial.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */

#ifndef PMATERIAL_H
#define PMATERIAL_H

#include <string>
#include <glm/glm.hpp>

#include "../gl/shaderprogram.h"

extern const std::string UM_ALBEDO;
extern const std::string UM_EMISSION;
extern const std::string UM_METALLIC;
extern const std::string UM_ROUGHNESS;

struct PMaterial
{
    PMaterial() {}
    PMaterial(glm::vec3 albedo, glm::vec3 emission,
              float metallic, float roughness)
        : albedo(albedo), emission(emission), metallic(metallic), roughness(roughness) {}

    glm::vec3 albedo;
    glm::vec3 emission;
    float metallic;
    float roughness;

    void updateUniforms(rgl::ShaderProgram& program) const;
};

extern const PMaterial DEFAULT_PMATERIAL;

#endif
