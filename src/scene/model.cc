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
    modelMatrix = node->worldTransform.toMat4();
    normalMatrix = glm::mat3(modelMatrix);
    modelMatrix = glm::scale(modelMatrix, scale);
}

void Model::updateMatrices(const glm::mat4& viewMatrix)
{
    updateMatrices();
    modelViewMatrix = viewMatrix * modelMatrix;
}

void Model::attachProgram(rgl::ShaderProgram* program)
{
    this->program = program;
}

void Model::render() const
{
    if (program != NULL)
        render(*program);
}

void Model::render(ShaderProgram& program) const
{
    if (program.hasUniform(U_MODELVIEWMATRIX))
        program.setUniformMatrix4f(U_MODELVIEWMATRIX, modelViewMatrix);
    if (program.hasUniform(U_NORMALMATRIX))
        program.setUniformMatrix3f(U_NORMALMATRIX, normalMatrix);
    program.setUniformMatrix4f(U_MODELMATRIX, modelMatrix);
    mesh->render();
}
