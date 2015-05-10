/**
 * @file noded.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-10
 */

#ifndef NODED_H
#define NODED_H

#include <glm/glm.hpp>
#include "math/tr3.h"
#include "math/movable3.h"
#include "node.h"

class Noded : public rm::Movable3
{
public:
    virtual ~Noded() {}

    void attachNode(Node* node, bool sync = true)
    {
        this->node = node;
        if (sync) {
            rm::Tr3 tX = rm::Movable3::getTransform();
            if (node->parent) {
                rm::Tr3 tP = node->parent->worldTransform;
                node->setTransform(multIA(tP, tX));
            }
        }
    }

    Node* getNode()
    {
        return node;
    }

    Node* detachNode(bool sync = true)
    {
        Node* returnNode = node;
        if (sync && node) {
            rm::Movable3::setTransform(node->worldTransform);
        }
        node = NULL;
        return returnNode;
    }

    bool hasNode()
    {
        return (node != NULL);
    }

    const rm::Tr3& getTransform() const
    {
        if (node)
            return node->getTransform();
        else
            return rm::Movable3::getTransform();
    }

    rm::Tr3& getTransform()
    {
        if (node)
            return node->getTransform();
        else
            return rm::Movable3::getTransform();
    }

    const rm::Tr3& getWorldTransform() const
    {
        if (node)
            return node->getWorldTransform();
        else
            return rm::Movable3::getTransform();
    }

    rm::Tr3& getWorldTransform()
    {
        if (node)
            return node->getWorldTransform();
        else
            return rm::Movable3::getTransform();
    }

    // Movement in world

    glm::vec3 getPositionW() const
    {
        return getWorldTransform().p;
    }

    glm::quat getRotationW() const
    {
        return getWorldTransform().q;
    }

    // World coordinate system

    glm::vec3 localToWorldW(const glm::vec3& p) const
    {
        return getWorldTransform() * p;
    }

    glm::vec3 worldToLocalW(const glm::vec3& p) const
    {
        return getWorldTransform().multI(p);
    }

    glm::vec3 localToWorldVW(const glm::vec3& v) const
    {
        return getWorldTransform().multV(v);
    }

    glm::vec3 worldToLocalVW(const glm::vec3& v) const
    {
        return getWorldTransform().multIV(v);
    }

    Node* node = NULL;
};

#endif
