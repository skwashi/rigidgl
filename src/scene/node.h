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

    rm::Tr3 worldTransform;

    void updateWorldTransform()
    {
        worldTransform = parent->worldTransform * getTransform();
    }

    void updateChildTransforms()
    {
        for (Node* node : children) {
            node->updateWorldTransform();
            node->updateChildTransforms();
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
