/**
 * @file material.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>

#include "../gl/shaderprogram.h"

extern const std::string UM_AMBIENT;
extern const std::string UM_DIFFUSE;
extern const std::string UM_SPECULAR;
extern const std::string UM_SHININESS;

struct Material
{
    Material() {}
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    void updateUniforms(rgl::ShaderProgram& program) const;
};

extern const Material DEFAULT_MATERIAL;

#endif
