/**
 * @file renderable.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-11
 */

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "shaderprogram.h"
#include "vertexattrib.h"

namespace rgl {

class Renderable
{
public:

    virtual ~Renderable() {}

    ShaderProgram* getProgram()
    {
        return program;
    }

    void attachProgram(ShaderProgram* program)
    {
        this->program = program;
    }

    const glm::mat4& getModelMatrix() const
    {
        return modelMatrix;
    }

    const glm::mat4& getModelViewMatrix() const
    {
        return modelViewMatrix;
    }

    const glm::vec3& getViewPosition() const
    {
        return viewPosition;
    }

    void updateMatrices(const glm::mat4& viewMatrix)
    {
        updateMatrices();
        modelViewMatrix = viewMatrix * modelMatrix;
        viewPosition = glm::vec3(modelViewMatrix[3]);
    }

    virtual void updateMatrices() = 0;

    virtual void updateUniforms(ShaderProgram& program) const
    {
        if (program.hasUniform(U_MODELVIEWMATRIX))
            program.setUniformMatrix4f(U_MODELVIEWMATRIX, modelViewMatrix);
        if (program.hasUniform(U_NORMALMATRIX))
            program.setUniformMatrix3f(U_NORMALMATRIX, normalMatrix);
        if (program.hasUniform(U_MODELMATRIX))
            program.setUniformMatrix4f(U_MODELMATRIX, modelMatrix);
    }

    virtual void render(ShaderProgram& program) const = 0;

    virtual void render() const
    {
        if (program != NULL) {
            program->use();
            render(*program);
        }
    }

    bool isEnabled() { return enabled; }
    void enable() { enabled = true; }
    void disable() { enabled = false; }
    void toggle() { enabled = !enabled; }

    glm::mat4 modelMatrix;
    glm::mat4 modelViewMatrix;
    glm::mat3 normalMatrix;
    glm::vec3 viewPosition;

protected:
    ShaderProgram* program;
    bool enabled = true;
};

} // namespace

#endif
