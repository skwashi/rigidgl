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

    virtual ~Movable3() {}
    virtual const Tr3& getTransform() const
    {
        return tX;
    }

    virtual Tr3& getTransform()
    {
        return tX;
    }

    bool hasMoved()
    {
        return moved;
    }

    void clearMoved()
    {
        moved = false;
    }

    void setTransform(const Tr3& tX)
    {
        getTransform() = tX;
        moved = true;
    }

    /* Movement */

    glm::vec3 getPosition() const
    {
        return getTransform().p;
    }

    glm::quat getRotation() const
    {
        return getTransform().q;
    }

    void translate(const glm::vec3& v)
    {
        getTransform().addTranslation(v);
        moved = true;
    }

    void translate(float x, float y, float z)
    {
        getTransform().addTranslation(x, y, z);
        moved = true;
    }

    void translateLocally(const glm::vec3& v)
    {
        getTransform() += v;
        moved = true;
    }

    void translateLocally(float x, float y, float z)
    {
        getTransform().addLocalTranslation(x, y, z);
        moved = true;
    }

    void rotate(float angle, const glm::vec3& axis)
    {
        getTransform().addRotation(angle, axis);
        moved = true;
    }

    void rotate(float angle, float x, float y, float z)
    {
        getTransform().addRotation(angle, x, y, z);
        moved = true;
    }

    void rotate(const glm::vec3& rot)
    {
        getTransform().addRotation(rot);
        moved = true;
    }

    void rotate(float x, float y, float z)
    {
        getTransform().addRotation(x, y, z);
        moved = true;
    }

    void rotateLocally(float angle, const glm::vec3& axis)
    {
        getTransform().addLocalRotation(angle, axis);
        moved = true;
    }

    void rotateLocally(float angle, float x, float y, float z)
    {
        getTransform().addLocalRotation(angle, x, y, z);
        moved = true;
    }

    void rotateLocally(const glm::vec3& rot)
    {
        getTransform().addLocalRotation(rot);
        moved = true;
    }

    void rotateLocally(float x, float y, float z)
    {
        getTransform().addLocalRotation(x, y, z);
        moved = true;
    }

    void rotateAround(const glm::vec3& pivot, float angle, const glm::vec3& axis)
    {
        getTransform().rotateAround(pivot, angle, axis);
        moved = true;
    }

    void rotateLocallyAround(const glm::vec3& pivot, float angle, const glm::vec3& axis)
    {
        getTransform().rotateLocallyAround(pivot, angle, axis);
        moved = true;
    }

    void moveTo(const glm::vec3& p)
    {
        getTransform().setPosition(p);
        moved = true;
    }

    void moveTo(float x, float y, float z)
    {
        getTransform().setPosition(x, y, z);
        moved = true;
    }

    void setRotation(float angle, const glm::quat& q)
    {
        getTransform().setRotation(q);
        moved = true;
    }

    void setRotation(float angle, const glm::vec3& axis)
    {
        getTransform().setRotation(angle, axis);
        moved = true;
    }

    void setRotation(float angle, float x, float y, float z)
    {
        getTransform().setRotation(angle, x, y, z);
        moved = true;
    }

    void setRotation(const glm::vec3& axis1, const glm::vec3& axis2, const glm::vec3& axis3)
    {
        getTransform().setRotation(axis1, axis2, axis3);
        moved = true;
    }

    void setOrientation(const glm::vec3& up, const glm::vec3& direction)
    {
        getTransform().setRotation(glm::cross(direction, up),up, -direction);
        moved = true;
    }

    void operator *=(const glm::quat& q)
    {
        getTransform() *= q;
        moved = true;
    }

    void operator +=(const glm::vec3& v)
    {
        getTransform() += v;
        moved = true;
    }

    /* Coordinate System */

    glm::vec3 localToWorld(const glm::vec3& p) const
    {
        return getTransform() * p;
    }

    glm::vec3 worldToLocal(const glm::vec3& p) const
    {
        return getTransform().multI(p);
    }

    glm::vec3 localToWorldV(const glm::vec3& v) const
    {
        return getTransform().multV(v);
    }

    glm::vec3 worldToLocalV(const glm::vec3& v) const
    {
        return getTransform().multIV(v);
    }

protected:
    rm::Tr3 tX;
    bool moved;
};

}


#endif
