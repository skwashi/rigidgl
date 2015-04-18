/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "pipeline.h"
#include "vertexattrib.h"
#include "shaderprogram.h"

using namespace glm;
using namespace rgl;

void Pipeline::update()
{
    projViewMatrix = projMatrix * viewMatrix;
}

void Pipeline::update(mat4 viewMatrix)
{
    this->viewMatrix = viewMatrix;
    projViewMatrix = projMatrix * viewMatrix;
}

void Pipeline::update(mat4 projMatrix, mat4 viewMatrix)
{
    this->projMatrix = projMatrix;
    this->viewMatrix = viewMatrix;
    projViewMatrix = projMatrix * viewMatrix;
}

void Pipeline::watchProgram(ShaderProgram& program)
{
    watchedPrograms.push_back(&program);
}

void Pipeline::clearPrograms()
{
    watchedPrograms.clear();
}

void Pipeline::updateMatrices(ShaderProgram& program) const
{
    if (program.hasUniform(U_PROJMATRIX))
        program.setUniformMatrix4f(U_PROJMATRIX, projMatrix);
    if (program.hasUniform(U_VIEWMATRIX))
        program.setUniformMatrix4f(U_VIEWMATRIX, viewMatrix);
    if (program.hasUniform(U_PROJVIEWMATRIX))
        program.setUniformMatrix4f(U_PROJVIEWMATRIX, projViewMatrix);
}

void Pipeline::updatePrograms()
{
    for (ShaderProgram* program : watchedPrograms) {
        program->use();
        updateMatrices(*program);
        program->disable();
    }
}
