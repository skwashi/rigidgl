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

#include "noded.h"
#include "../gl/shaderprogram.h"
#include "../gl/vertexbuffer.h"
#include "../gl/renderable.h"

extern const std::string UL_TYPE;
extern const std::string UL_POSITION;
extern const std::string UL_DIRECTION;
extern const std::string UL_COLOR;
extern const std::string UL_INTENSITY;
extern const std::string UL_ATTENUATION;
extern const std::string UL_CONEANGLE;
extern const std::string UL_RADIUS;

class Light : public Noded
{
public:

    enum Type
    {
        POINT = 0,
        DIRECTIONAL = 1,
        SPOT = 2
    };

    Light() {}

    Light(const glm::vec3& color,
          const glm::vec3& intensity,
          const glm::vec3& attenuation)
        : color(color), intensity(intensity), attenuation(attenuation) {}

    Light(const glm::vec3& color,
          const glm::vec3 intensity,
          float attenuation)
        : color(color), intensity(intensity)
    {
        this->attenuation =
            glm::vec3(1.0f, attenuation, attenuation * attenuation);
    }

    Light(const glm::vec3& color, float ambientIntensity,
          float diffuseIntensity, float specularIntensity,
          glm::vec3 attenuation)
        : color(color),
          intensity(ambientIntensity, diffuseIntensity, specularIntensity),
          attenuation(attenuation) {}

    Light(const glm::vec3& color, float ambientIntensity,
          float diffuseIntensity, float specularIntensity,
          float attenuation)
        : color(color),
          intensity(ambientIntensity, diffuseIntensity, specularIntensity)
    {
        this->attenuation =
            glm::vec3(1.0f, attenuation, attenuation * attenuation);
    }

    void setPoint(float radius = 1.0f)
    {
        type = POINT;
        this->radius = radius;
    }

    void setDirectional(const glm::vec3& direction)
    {
        type = DIRECTIONAL;
        this->direction = direction;
    }

    void setSpot(const glm::vec3& direction, float coneAngle,
        float radius = 1.0f)
    {
        type = SPOT;
        this->direction = direction;
        this->coneAngle = coneAngle;
        this->radius = radius;
    }

    void updateUniforms(rgl::ShaderProgram& program, int i = -1) const;

    void updateUniforms(const rm::Tr3& tX,
                        rgl::ShaderProgram& program, int i = -1) const;

    void render(rgl::ShaderProgram& program) const;

    bool isEnabled() { return enabled; }
    void enable() { enabled = true; }
    void disable() { enabled = false; }
    void toggle() { enabled = !enabled; }

    Type type = POINT;

    glm::vec3 color;
    glm::vec3 intensity;
    glm::vec3 attenuation;

    glm::vec3 direction = glm::vec3(0, 0, -1);
    float coneAngle = 0;
    float radius = 1;

private:
    void updateCommonUniforms(const std::string& prefix,
                              rgl::ShaderProgram& program) const;

    bool enabled = true;

};


#endif
