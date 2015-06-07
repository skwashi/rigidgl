/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "model.h"

#include "math/movable3.h"
#include "../gl/rgl.h"
#include "../gl/mesh.h"
#include "../gl/shaderprogram.h"

using namespace rgl;

void Model::updateMatrices()
{
    modelMatrix = getWorldTransform().toMat4();
    normalMatrix = glm::mat3(modelMatrix);
    modelMatrix = glm::scale(modelMatrix, scale);
}

void Model::updateUniforms(ShaderProgram& program) const
{
    Renderable::updateUniforms(program);
    material.updateUniforms(program);
    pmaterial.updateUniforms(program);
}

void Model::render(ShaderProgram& program)
{
    updateUniforms(program);
    if (texture)
        texture->bind();
    if (mesh)
        mesh->render();
}
