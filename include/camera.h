/**
 * @author Jonas Ransjö
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "math/movable3.h"

class Camera : public rm::Movable3
{
public:

    Camera(float fov, float aspectRatio, float near, float far) :
        fov(fov), aspectRatio(aspectRatio), near(near), far(far)
    {
        computePerspective();
    }

    void setFov(float _fov)
    {
        fov = _fov;
        computePerspective();
    }

    void set(float _fov, float _aspectRatio, float _near, float _far)
    {
        fov = _fov;
        aspectRatio = _aspectRatio;
        near= _near;
        far = _far;
        computePerspective();
    }

    glm::mat4 getViewMatrix() const
    {
        return viewMatrix;
    }

    glm::mat4 getProjectionMatrix() const
    {
        return projectionMatrix;
    }

    glm::mat4 computeViewMatrix()
    {
        viewMatrix = tX.toMat4I();
        return viewMatrix;
    }

    void update()
    {
        computeViewMatrix();
    }

private:
    float fov;
    float aspectRatio;
    float near;
    float far;
    float yRatio;
    float xRatio;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void computePerspective()
    {
        yRatio = tan(fov / 2);
        xRatio = yRatio * aspectRatio;
        projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
    }

};


#endif
