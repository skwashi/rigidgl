/**
 * @author Jonas Ransjö
 */

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "rtypes.h"
#include "math/movable3.h"
#include "vertexattrib.h"
#include "vertexbuffer.h"
#include "shaderprogram.h"
#include "batch.h"

using namespace rgl;

Batch::Batch(const std::vector<VertexAttrib>& attribs,
          int vertexCapacity, int indexCapacity)
{
    vertexBuffer.init(attribs, GL_STREAM_DRAW);
    vertexBuffer.reserve(vertexCapacity, indexCapacity);
}

bool Batch::isEmpty() const
{
    return (vertexCount == 0);
}

void Batch::setColor(const Color& color)
{
    oldColor = color;
    this->color = color;
}

void Batch::setColor(float r, float g, float b, float a)
{
    oldColor = color;
    color = Color(r, g, b, a);
}

void Batch::unsetColor()
{
    color = oldColor;
}

void Batch::attachProgram(ShaderProgram* program)
{
    this->program = program;
}

void Batch::begin()
{
    state = BATCHING;
}

void Batch::end()
{
    flush();
    state = IDLE;
}

void Batch::clear()
{
    vertexBuffer.clear();
    vertexCount = 0;
    indexCount = 0;
}

void Batch::finalize()
{
    vertexBuffer.bufferData(GL_STATIC_DRAW);
    state = IDLE;
}

void Batch::flush()
{
    vertexBuffer.render();
    clear();
}

void Batch::updateMatrices()
{
    modelMatrix = getTransform().toMat4();
}

void Batch::render(GLenum mode)
{
    bind();
    if (program != NULL) {
        program->use();
        render(program, mode);
    }
    else
        vertexBuffer.render();
}

void Batch::render(ShaderProgram* program, GLenum mode)
{
    if (program->hasUniform(U_MODELMATRIX))
        program->setUniformMatrix4f(U_MODELMATRIX, modelMatrix);
    vertexBuffer.render();
}
