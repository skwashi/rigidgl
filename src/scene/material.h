/**
 * @file material.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

const std::string UM = "material";
const std::string UM_AMBIENT = "ambient";
const std::string UM_DIFFUSE = "diffuse";
const std::string UM_SPECULAR = "specular";
const std::string UM_SHININESS = "shininess"

struct Material
{
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
             float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          shininess(shininess) {}

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

#endif
