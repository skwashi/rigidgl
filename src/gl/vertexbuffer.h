/**
 * @author Jonas Ransjö
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <vector>
#include <GL/glew.h>

#include "rtypes.h"
#include "vertexarray.h"
#include "vertexattrib.h"

namespace rgl {

class VertexBuffer
{
public:
    struct Item {
        uint vstart;
        uint vcount;
        uint istart;
        uint icount;
    };

    enum State {
        CLEAN = 0,
        DIRTY = 1,
        BUFFERING = 2
    };

    VertexBuffer() {}

    VertexBuffer(const std::vector<VertexAttrib>& attribs)
        : VertexBuffer(attribs, GL_STATIC_DRAW) {}

    VertexBuffer(const std::vector<VertexAttrib>& attribs, GLenum usage);

    VertexBuffer(const VertexAttrib* attribs, uint count)
        : VertexBuffer(attribs, count, GL_STATIC_DRAW) {}

    VertexBuffer(const VertexAttrib* attribs, uint count, GLenum usage);

    ~VertexBuffer() {}

    void init(const std::vector<VertexAttrib>& attribs, GLenum usage = GL_STATIC_DRAW);

    void init(const VertexAttrib* attribs, uint count, GLenum usage = GL_STATIC_DRAW);

    void useIndices(bool flag);
    void setUsage(GLenum usage);
    Item getItem(uint index) const;
    uint getVertexCount() const;
    uint getIndexCount() const;
    uint getItemCount() const;
    bool isUsingIndices() const;
    bool isInited() const;

    void reserve(uint vcount);
    void reserve(uint vcount, uint icount);
    void clearVertices();
    void clearIndices();
    void clear();
    void pushVertices(float* vertices, uint vcount);
    void pushFloat(float f);
    void pushIndices(uint* indices, uint icount);
    void pushIndex(uint index);

    void addTriangle(uint v1, uint v2, uint v3);
    void addTriangle(uint offset);
    void addQuad(uint v1, uint v2, uint v3, uint v4);
    void addQuad(uint offset);

    void push(float* vertices, uint vcount, unsigned int* indices, uint icount);
    uint pushItem(float* vertices, uint vcount, unsigned int* indices, uint icount);

    void bind() const;
    void bufferData(GLenum usage);
    void render(GLenum mode = GL_TRIANGLES);
    void renderItem(uint index, GLenum mode = GL_TRIANGLES);
    void renderItems(uint index, uint count, GLenum mode = GL_TRIANGLES);

protected:
    VertexArray vertexArray;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<Item> items;
    uint vsize;
    State state = CLEAN;
    GLenum usage;
    bool usingIndices;
};

inline void VertexBuffer::pushVertices(float* _vertices, uint count)
{
    vertices.insert(vertices.end(), _vertices, _vertices + count);
    state = DIRTY;
}

inline void VertexBuffer::pushFloat(float f)
{
    vertices.push_back(f);
    state = DIRTY;
}

inline void VertexBuffer::pushIndices(uint* _indices, uint count)
{
    indices.insert(indices.end(), _indices, _indices + count);
    state = DIRTY;
}

inline void VertexBuffer::pushIndex(uint index)
{
    indices.push_back(index);
    state = DIRTY;
}

inline void VertexBuffer::addTriangle(uint v1, uint v2, uint v3)
{
    indices.push_back(v1);
    indices.push_back(v2);
    indices.push_back(v3);

    state = DIRTY;
}

inline void VertexBuffer::addTriangle(uint offset)
{
    indices.push_back(offset);
    indices.push_back(offset + 1);
    indices.push_back(offset + 2);

    state = DIRTY;
}

inline void VertexBuffer::addQuad(uint v1, uint v2, uint v3, uint v4)
{
    addTriangle(v1, v2, v3);
    addTriangle(v1, v3, v4);
}

inline void VertexBuffer::addQuad(uint offset)
{
    addTriangle(offset, offset + 1, offset + 2);
    addTriangle(offset, offset + 2, offset + 3);
}

} // namespace

#endif
