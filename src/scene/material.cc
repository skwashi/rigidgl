/**
 * @file material.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-12
 */



#include <string>
#include <glm/glm.hpp>

#include "material.h"
#include "../gl/shaderprogram.h"

const std::string UM_AMBIENT = "mat.ambient";
const std::string UM_DIFFUSE = "mat.diffuse";
const std::string UM_SPECULAR = "mat.specular";
const std::string UM_SHININESS = "mat.shininess";

const Material DEFAULT_MATERIAL = Material(
    glm::vec3(0.01, 0.01, 0.01),
    glm::vec3(0.8, 0.8, 0.8),
    glm::vec3(0.2, 0.2, 0.2),
    24.f);

void Material::updateUniforms(rgl::ShaderProgram& program) const
{
    if (program.hasUniform(UM_AMBIENT))
        program.setUniform3f(UM_AMBIENT, ambient);

    if (program.hasUniform(UM_DIFFUSE))
        program.setUniform3f(UM_DIFFUSE, diffuse);

    if (program.hasUniform(UM_SPECULAR))
        program.setUniform3f(UM_SPECULAR, specular);

    if (program.hasUniform(UM_SHININESS))
        program.setUniform1f(UM_SHININESS, shininess);
}
