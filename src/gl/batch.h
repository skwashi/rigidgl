/**
 * @author Jonas Ransjö
 */

#ifndef BATCH_H
#define BATCH_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "rtypes.h"
#include "math/movable3.h"
#include "vertexattrib.h"
#include "vertexbuffer.h"
#include "shaderprogram.h"

namespace rgl {

class Batch : public rm::Movable3
{
public:

    enum State {
        IDLE = 0,
        BATCHING = 1
    };

    Batch(const std::vector<VertexAttrib>& attribs,
          int vertexCapacity, int indexCapacity);

    bool isEmpty() const;
    void setColor(const Color& color);
    void setColor(float r, float g, float b, float a);
    void unsetColor();
    void attachProgram(ShaderProgram* program);
    void begin();
    void end();
    void clear();
    void finalize();
    void flush();
    void updateMatrices();
    virtual void render(GLenum mode = GL_TRIANGLES);
    virtual void render(ShaderProgram* program, GLenum mode = GL_TRIANGLES);

    void addTriangle();
    void addTriangle(uint offset);
    void addTriangle(uint v1, uint v2, uint v3);
    void addQuad();
    void addQuad(uint offset);
    void addQuad(uint v1, uint v2, uint v3, uint v4);

protected:
    VertexBuffer vertexBuffer;
    int vertexCapacity;
    int indexCapacity;
    int vertexCount = 0;
    int indexCount = 0;
    State state = IDLE;
    Color color;
    Color oldColor;

    glm::mat4 modelMatrix;
    ShaderProgram* program;

    virtual void bind() const;
    virtual void flushCheck(int newCount = 0);
};

inline void Batch::addTriangle()
{
    vertexBuffer.addTriangle(vertexCount);
}

inline void Batch::addTriangle(uint offset) {
    vertexBuffer.addTriangle(offset);
}

inline void Batch::addTriangle(uint v1, uint v2, uint v3)
{
    vertexBuffer.addTriangle(vertexCount + v1, vertexCount + v2, vertexCount + v3);
}

inline void Batch::addQuad()
{
    vertexBuffer.addQuad(vertexCount);
}

inline void Batch::addQuad(uint offset)
{
    vertexBuffer.addQuad(offset);
}

inline void Batch::addQuad(uint v1, uint v2, uint v3, uint v4)
{
    vertexBuffer.addQuad(vertexCount + v1, vertexCount + v2,
                         vertexCount + v3, vertexCount + v4);
}

inline void Batch::bind() const
{
    vertexBuffer.bind();
}

inline void Batch::flushCheck(int newCount)
{
    if (vertexCount + newCount > vertexCapacity)
        flush();
}

}

#endif
