/**
 * @file light.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-06
 */

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <string>

#include "math/movable3.h"
#include "math/tr3.h"
#include "../gl/shaderprogram.h"

const std::string UL_TYPE = "type";
const std::string UL_POSITION = "position";
const std::string UL_DIRECTION = "direction";
const std::string UL_AMBIENT = "ambient";
const std::string UL_DIFFUSE = "diffuse";
const std::string UL_SPECULAR = "specular";
const std::string UL_ATTENUATION = "attenuation";
const std::string UL_CONEANGLE = "coneAngle";

class Light : public rm::Movable3
{
public:

    enum Type
    {
        POINT = 0,
        DIRECTIONAL = 1,
        SPOT = 2
    };

    Light() {}

    Light(const glm::vec3& ambient,
          const glm::vec3& diffuse,
          const glm::vec3& specular,
          const glm::vec3& attenuation)
        : ambient(ambient), diffuse(diffuse),
          specular(specular), attenuation(attenuation) {}

    Light(const glm::vec3& ambient,
          const glm::vec3& diffuse,
          const glm::vec3& specular,
          float attenuation)
        : ambient(ambient), diffuse(diffuse),
          specular(specular)
    {
        this->attenuation =
            glm::vec3(1.0f, attenuation, attenuation * attenuation);
    }

    Light(const glm::vec3& color, float ambientIntensity,
          float diffuseIntensity, float specularIntensity,
          glm::vec3 attenuation)
    {
        ambient = color * ambientIntensity;
        diffuse = color * diffuseIntensity;
        specular = color * specularIntensity;
        this->attenuation = attenuation;
    }

    Light(const glm::vec3& color, float ambientIntensity,
          float diffuseIntensity, float specularIntensity,
          float attenuation)
    {
        ambient = color * ambientIntensity;
        diffuse = color * diffuseIntensity;
        specular = color * specularIntensity;
        this->attenuation =
            glm::vec3(1.0f, attenuation, attenuation * attenuation);
    }

    void setPoint()
    {
        type = POINT;
    }

    void setDirectional(const glm::vec3& direction)
    {
        type = DIRECTIONAL;
        this->direction = direction;
    }

    void setSpot(const glm::vec3& direction, float coneAngle)
    {
        type = SPOT;
        this->direction = direction;
        this->coneAngle = coneAngle;
    }

    void updateUniforms(rgl::ShaderProgram& program, int i = -1) const;

    void updateUniforms(const rm::Tr3& tX,
                        rgl::ShaderProgram& program, int i = -1) const;

    Type type = POINT;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 attenuation;

    glm::vec3 direction = glm::vec3(0, 0, -1);
    float coneAngle = 0;

private:
    void updateCommonUniforms(const std::string& prefix,
                              rgl::ShaderProgram& program) const;

};


#endif
