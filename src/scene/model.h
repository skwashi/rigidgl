/**
 * @author Jonas Ransjö
 */

#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <glm/glm.hpp>

#include "math/tr3.h"
#include "math/movable3.h"
#include "node.h"
#include "../gl/rgl.h"
#include "../gl/shaderprogram.h"
#include "../gl/mesh.h"


class Model : public rm::Movable3
{
public:
    Model(Node* node,
          const std::shared_ptr<rgl::Mesh>& mesh) : node(node), mesh(mesh) {}

    Node* node;
    std::shared_ptr<rgl::Mesh> mesh;
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::mat4 modelMatrix;
    glm::mat4 modelViewMatrix;
    glm::mat3 normalMatrix;

    virtual const rm::Tr3& getTransform() const
    {
        return node->getTransform();
    }

    virtual rm::Tr3& getTransform()
    {
        return node->getTransform();
    }

    void updateMatrices();
    void updateMatrices(const glm::mat4& viewMatrix);
    void attachProgram(rgl::ShaderProgram* program);
    void render() const;
    void render(rgl::ShaderProgram& program) const;

private:
    rgl::ShaderProgram* program = NULL;
};


#endif
