/**
 * @author Jonas Ransjö
 */

#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>

#include "math/tr3.h"
#include "math/movable3.h"
#include "node.h"
#include "noded.h"
#include "../gl/rgl.h"
#include "../gl/shaderprogram.h"
#include "../gl/mesh.h"
#include "../gl/gltexture.h"
#include "../gl/renderable.h"
#include "material.h"
#include "pmaterial.h"

class Model : public Noded, public rgl::Renderable
{
public:
    Model(rgl::Mesh* mesh) : mesh(mesh) {}
    Model(Node* node,
          rgl::Mesh* mesh)
        : mesh(mesh) { attachNode(node); }
    Model(rgl::Mesh* mesh,
          rgl::GLTexture* texture)
        : mesh(mesh), texture(texture) {}
    Model(Node* node,
          rgl::Mesh* mesh,
          rgl::GLTexture* texture)
        : mesh(mesh), texture(texture) { attachNode(node); }

    ~Model() {}

    rgl::GLTexture* getTexture() { return texture; }
    rgl::Mesh* getMesh() { return mesh; }

    void setMesh(rgl::Mesh* mesh) { this->mesh = mesh; }
    void setTexture(rgl::GLTexture* texture) { this->texture = texture; }
    void setMaterial(Material material) { this->material = material; }
    void setMaterial(PMaterial pmaterial) { this->pmaterial = pmaterial; }

    // virtual in renderable
    using rgl::Renderable::updateMatrices;
    void updateMatrices();
    void updateUniforms(rgl::ShaderProgram& program) const;
    void render(rgl::ShaderProgram& program);
    using rgl::Renderable::render;

    glm::vec3 scale = glm::vec3(1, 1, 1);

private:
    rgl::Mesh* mesh = NULL;
    rgl::GLTexture* texture = NULL;
    Material material = DEFAULT_MATERIAL;
    PMaterial pmaterial = DEFAULT_PMATERIAL;

};


#endif
