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

void Model::setTexture(GLTexture* texture)
{
    this->texture = texture;
}

void Model::attachProgram(rgl::ShaderProgram* program)
{
    this->program = program;
}

void Model::updateMatrices()
{
    if (node)
        modelMatrix = node->worldTransform.toMat4();
    else
        modelMatrix = getTransform().toMat4();
    normalMatrix = glm::mat3(modelMatrix);
    modelMatrix = glm::scale(modelMatrix, scale);
}

void Model::updateMatrices(const glm::mat4& viewMatrix)
{
    updateMatrices();
    modelViewMatrix = viewMatrix * modelMatrix;
}

void Model::render() const
{
    if (program != NULL) {
        program->use();
        render(*program);
    }
}

void Model::render(ShaderProgram& program) const
{
    if (program.hasUniform(U_MODELVIEWMATRIX))
        program.setUniformMatrix4f(U_MODELVIEWMATRIX, modelViewMatrix);
    if (program.hasUniform(U_NORMALMATRIX))
        program.setUniformMatrix3f(U_NORMALMATRIX, normalMatrix);
    if (program.hasUniform(U_MODELMATRIX))
        program.setUniformMatrix4f(U_MODELMATRIX, modelMatrix);
    if (texture)
        texture->bind();
    if (mesh)
        mesh->render();
}
