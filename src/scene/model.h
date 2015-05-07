/**
 * @author Jonas Ransjö
 */

#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>

#include "math/tr3.h"
#include "math/movable3.h"
#include "node.h"
#include "../gl/rgl.h"
#include "../gl/shaderprogram.h"
#include "../gl/mesh.h"
#include "../gl/gltexture.h"

class Model : public rm::Movable3
{
public:
    Model(rgl::Mesh* mesh) : mesh(mesh) {}
    Model(Node* node,
          rgl::Mesh* mesh)
        : node(node), mesh(mesh) {}
    Model(rgl::Mesh* mesh,
          rgl::GLTexture* texture)
        : mesh(mesh), texture(texture) {}
    Model(Node* node,
          rgl::Mesh* mesh,
          rgl::GLTexture* texture)
        : node(node), mesh(mesh), texture(texture) {}

    Node* node = NULL;
    rgl::Mesh* mesh = NULL;
    rgl::GLTexture* texture = NULL;
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::mat4 modelMatrix;
    glm::mat4 modelViewMatrix;
    glm::mat3 normalMatrix;

    virtual const rm::Tr3& getTransform() const
    {
        if (node)
            return node->getTransform();
        else
            return Movable3::getTransform();
    }

    virtual rm::Tr3& getTransform()
    {
        if (node)
            return node->getTransform();
        else
            return Movable3::getTransform();
    }

    void setTexture(rgl::GLTexture* texture);
    void attachProgram(rgl::ShaderProgram* program);
    void updateMatrices();
    void updateMatrices(const glm::mat4& viewMatrix);
    void render() const;
    void render(rgl::ShaderProgram& program) const;

private:
    rgl::ShaderProgram* program = NULL;
};


#endif
