/**
 * @author Jonas Ransjö
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "math/tr3.h"
#include "math/movable3.h"

class Camera : public rm::Movable3
{
public:
    void init(float fov, float aspectRatio, float near, float far);
    float getFov() const {return fov;}
    float getAspectRatio() const {return aspectRatio;}
    float getNear() const {return near;};
    float getFar() const {return far;}
    float getXRatio() const { return xRatio; }
    float getYRatio() const { return yRatio; }
    void setFov(float fov);
    void setAspectRatio(float aspectRatio);
    void set(float fov, float aspectRatio, float near, float far);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 computeViewMatrix();
    void update();

private:
    float fov;
    float aspectRatio;
    float near;
    float far;
    float yRatio;
    float xRatio;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void computePerspective();
};


#endif
