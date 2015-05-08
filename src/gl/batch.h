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

    Batch(const std::vector<VertexAttrib>& attribs);

    Batch(const std::vector<VertexAttrib>& attribs,
          int vertexCapacity, int indexCapacity, bool flushing = true);

    bool isEmpty() const;
    void setColor(const Color& color);
    void setColor(float r, float g, float b, float a);
    void unsetColor();
    void attachProgram(ShaderProgram* program);
    ShaderProgram* getProgram();
    void begin();
    void end();
    void clear();
    void finalize();
    void flush();
    void updateMatrices();
    virtual void render(GLenum mode = GL_TRIANGLES);
    virtual void render(ShaderProgram* program, GLenum mode = GL_TRIANGLES);

    void addTriangleI();
    void addTriangleI(uint offset);
    void addTriangleI(uint v1, uint v2, uint v3);
    void addQuadI();
    void addQuadI(uint offset);
    void addQuadI(uint v1, uint v2, uint v3, uint v4);

protected:
    VertexBuffer vertexBuffer;
    int vertexCapacity;
    int indexCapacity;
    bool flushing;
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

inline void Batch::addTriangleI()
{
    vertexBuffer.addTriangleI(vertexCount);
}

inline void Batch::addTriangleI(uint offset) {
    vertexBuffer.addTriangleI(offset);
}

inline void Batch::addTriangleI(uint v1, uint v2, uint v3)
{
    vertexBuffer.addTriangleI(vertexCount + v1, vertexCount + v2, vertexCount + v3);
}

inline void Batch::addQuadI()
{
    vertexBuffer.addQuadI(vertexCount);
}

inline void Batch::addQuadI(uint offset)
{
    vertexBuffer.addQuadI(offset);
}

inline void Batch::addQuadI(uint v1, uint v2, uint v3, uint v4)
{
    vertexBuffer.addQuadI(vertexCount + v1, vertexCount + v2,
                         vertexCount + v3, vertexCount + v4);
}

inline void Batch::bind() const
{
    vertexBuffer.bind();
}

inline void Batch::flushCheck(int newCount)
{
    if (flushing && (vertexCount + newCount > vertexCapacity))
        flush();
}

}

#endif
