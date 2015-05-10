/**
 * @author Jonas Ransjö
 */

#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <vector>

#include "math/tr3.h"
#include "math/movable3.h"

struct Node : public rm::Movable3
{
    Node* parent = NULL;
    std::vector<Node*> children;

    glm::vec3 v;
    glm::vec3 w;

    rm::Tr3 worldTransform;

    const rm::Tr3& getWorldTransform() const
    {
        return worldTransform;
    }

    rm::Tr3& getWorldTransform()
    {
        return worldTransform;
    }

    void updateWorldTransform()
    {
        if (parent != NULL)
            worldTransform = parent->worldTransform * getTransform();
        else
            worldTransform = getTransform();
    }

    void updateTransforms(float dt)
    {
        getTransform().addTranslation(v * dt);
        getTransform().addRotation(w * dt);
        updateWorldTransform();
    }

    void updateChildTransforms()
    {
        for (Node* node : children) {
            node->updateWorldTransform();
            node->updateChildTransforms();
        }
    }

    void updateChildTransforms(float dt)
    {
        for (Node* node : children) {
            node->updateTransforms(dt);
            node->updateChildTransforms(dt);
        }
    }

    bool releaseChild(const Node* node)
    {
        auto pos = std::find(std::begin(children), std::end(children), node);
        if (pos == children.end())
            return false;
        children.erase(pos);
        return true;
    }

    void releaseAllChildren()
    {
        children.clear();
    }

    void attachChild(Node* node)
    {
        children.push_back(node);
    }

};

#endif
