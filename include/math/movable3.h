/**
 * @author Jonas Ransjö
 */

#ifndef MOVABLE3_H
#define MOVABLE3_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "rmath.h"
#include "tr3.h"

namespace rm {

class Movable3
{
public:

    Tr3 getTransform() const {
        return tX;
    };

    /* Movement */

    glm::vec3 getPosition() const
    {
        return tX.p;
    }

    glm::quat getRotation() const
    {
        return tX.q;
    }

    void translate(const glm::vec3& v)
    {
        tX.addTranslation(v);
    }

    void translate(float x, float y, float z)
    {
        tX.addTranslation(x, y, z);
    }

    void translateLocally(const glm::vec3& v)
    {
        tX += v;
    }

    void translateLocally(float x, float y, float z)
    {
        tX.addLocalTranslation(x, y, z);
    }

    void rotate(float angle, const glm::vec3& axis)
    {
        tX.addRotation(angle, axis);
    }

    void rotate(float angle, float x, float y, float z)
    {
        tX.addRotation(angle, x, y, z);
    }

    void rotateLocally(float angle, const glm::vec3& axis)
    {
        tX.addLocalRotation(angle, axis);
    }

    void rotateLocally(float angle, float x, float y, float z)
    {
        tX.addLocalRotation(angle, x, y, z);
    }

    void rotateAround(const glm::vec3& pivot, float angle, const glm::vec3& axis)
    {
        tX.rotateAround(pivot, angle, axis);
    }

    void rotateLocallyAround(const glm::vec3& pivot, float angle, const glm::vec3& axis)
    {
        tX.rotateLocallyAround(pivot, angle, axis);
    }

    void moveTo(const glm::vec3& p)
    {
        tX.setPosition(p);
    }

    void moveTo(float x, float y, float z)
    {
        tX.setPosition(x, y, z);
    }

    void setRotation(float angle, const glm::quat& q)
    {
        tX.setRotation(q);
    }

    void setRotation(float angle, const glm::vec3& axis)
    {
        tX.setRotation(angle, axis);
    }

    void setRotation(float angle, float x, float y, float z)
    {
        tX.setRotation(angle, x, y, z);
    }

    void setRotation(const glm::vec3& axis1, const glm::vec3& axis2, const glm::vec3& axis3)
    {
        tX.setRotation(axis1, axis2, axis3);
    }

    void setOrientation(const glm::vec3& up, const glm::vec3& direction)
    {
        tX.setRotation(glm::cross(direction, up),up, -direction);
    }

    void operator *=(const glm::quat& q)
    {
        tX *= q;
    }

    void operator +=(const glm::vec3& v)
    {
        tX += v;
    }

    /* Coordinate System */

    glm::vec3 localToWorld(const glm::vec3& p)
    {
        return tX * p;
    }

    glm::vec3 worldToLocal(const glm::vec3& p)
    {
        return tX.multI(p);
    }

    glm::vec3 localToWorldV(const glm::vec3& v)
    {
        return tX.multV(v);
    }

    glm::vec3 worldToLocalV(const glm::vec3& v)
    {
        return tX.multIV(v);
    }

protected:
    Tr3 tX;
};

}


#endif
