/**
 * @author Jonas Ransjö
 */

#include <glm/glm.hpp>
#include <vector>

#include "math/tr3.h"
#include "math/movable3.h"
#include "node.h"
#include "scenegraph.h"


SceneGraph::SceneGraph()
{
    root = new Node;
    nodes.push_back(root);
}

SceneGraph::~SceneGraph()
{
    for (Node* node : nodes)
        delete node;
}

Node* SceneGraph::allocateNode()
{
    Node* node = new Node;
    nodes.push_back(node);
    attachNode(node, root);
    return node;
}

bool SceneGraph::freeNode(Node* node)
{
    releaseNode(node);
    auto pos = std::find(std::begin(nodes), std::end(nodes), node);

    if (pos == nodes.end())
        return false;
    nodes.erase(pos);
    delete node;
    return true;
}

void SceneGraph::releaseNode(Node* node)
{
    if (node->parent != NULL)
        node->parent->releaseChild(node);
}

void SceneGraph::releaseNodeInWorld(Node* node)
{
    releaseNode(node);
    node->getTransform() = node->worldTransform;
}

void SceneGraph::attachNode(Node* node)
{
    if (node->parent != NULL)
        node->parent->releaseChild(node);
    node->parent = root;
    root->attachChild(node);
    node->worldTransform = root->worldTransform * node->getTransform();
}

void SceneGraph::attachNode(Node* node, Node* parent)
{
    if (node->parent != NULL)
        node->parent->releaseChild(node);
    node->parent = parent;
    node->parent->attachChild(node);
    node->worldTransform = parent->worldTransform * node->getTransform();
}

void SceneGraph::attachNodeInWorld(Node* node, Node* parent)
{
    node->worldTransform = node->getTransform();
    node->getTransform() = rm::multIA(parent->worldTransform, node->getTransform());
}

void SceneGraph::updateTransforms()
{
    root->worldTransform = root->getTransform();
    root->updateChildTransforms();
}
