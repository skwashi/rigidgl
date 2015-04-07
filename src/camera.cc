/**
 * @author Jonas Ransjö
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "math/tr3.h"
#include "math/movable3.h"
#include "camera.h"

void Camera::init(float fov, float aspectRatio, float near, float far)
{
    set(fov, aspectRatio, near, far);
    viewMatrix = glm::mat4();
}

void Camera::setFov(float _fov)
{
    fov = _fov;
    computePerspective();
}

void Camera::setAspectRatio(float _aspectRatio)
{
    aspectRatio = _aspectRatio;
    computePerspective();
}

void Camera::set(float _fov, float _aspectRatio, float _near, float _far)
{
    fov = _fov;
    aspectRatio = _aspectRatio;
    near= _near;
    far = _far;
    computePerspective();
}

glm::mat4 Camera::getViewMatrix() const
{
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return projectionMatrix;
}

glm::mat4 Camera::computeViewMatrix()
{
    viewMatrix = tX.toMat4I();
    return viewMatrix;
}

void Camera::update()
{
    computeViewMatrix();
}

void Camera::computePerspective()
{
    yRatio = tan(fov / 2);
    xRatio = yRatio * aspectRatio;
    projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
}
