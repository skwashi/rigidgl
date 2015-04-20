/**
 * @author Jonas Ransjö
 */

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <glm/glm.hpp>
#include <vector>

#include "math/tr3.h"
#include "math/movable3.h"
#include "node.h"

class SceneGraph : public rm::Movable3
{
public:
    SceneGraph();
    ~SceneGraph();

    Node* allocateNode();
    bool freeNode(Node* node);
    void releaseNode(Node* node);
    void releaseNodeInWorld(Node* node);
    void attachNode(Node* node);
    void attachNode(Node* node, Node* parent);
    void attachNodeInWorld(Node* node, Node* parent);
    void updateTransforms();
    void updateTransforms(float dt);

    virtual const rm::Tr3& getTransform() const
    {
        return root->getTransform();
    }

    virtual rm::Tr3& getTransform()
    {
        return root->getTransform();
    }

private:
    std::vector<Node*> nodes;
    Node* root = NULL;
};


#endif
