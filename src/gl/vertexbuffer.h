/**
 * @author Jonas Ransjö
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rtypes.h"
#include "math/glmutils.h"
#include "vertexarray.h"
#include "vertexattrib.h"
#include "vertex.h"

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
    void reserveF(uint fcount);
    void reserveF(uint fcount, uint icount);
    void clearVertices();
    void clearIndices();
    void clear();
    void pushFloats(const float* floats, uint fcount);
    void pushFloat(float f);
    void pushIndices(const uint* indices, uint icount);
    void pushIndex(uint index);

    template <typename T>
    void pushVector(const T& v);

    template <typename T>
    void pushVertex(const T& vertex);

    void pushVertices(const float* floats, uint fcount);

    template <typename T>
    void pushVertices(const T* vertices, uint vcount);

    void push(const float* floats, uint fcount, const unsigned int* indices, uint icount);
    uint pushItem(const float* floats, uint fcount, const unsigned int* indices, uint icount);

    template <typename T>
    void push(const T* vertices, uint vcount, const uint* indices, uint icount);

    template <typename T>
    void pushItem(const T* vertices, uint vcount, const uint* indices, uint icount);

    void addTriangleI(uint v1, uint v2, uint v3);
    void addTriangleI(uint offset);
    void addQuadI(uint v1, uint v2, uint v3, uint v4);
    void addQuadI(uint offset);

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

inline void VertexBuffer::pushFloats(const float* floats, uint count)
{
    vertices.insert(vertices.end(), floats, floats + count);
    state = DIRTY;
}

inline void VertexBuffer::pushFloat(float f)
{
    vertices.push_back(f);
    state = DIRTY;
}

inline void VertexBuffer::pushIndices(const uint* _indices, uint count)
{
    indices.insert(indices.end(), _indices, _indices + count);
    state = DIRTY;
}

inline void VertexBuffer::pushIndex(uint index)
{
    indices.push_back(index);
    state = DIRTY;
}

inline void VertexBuffer::addTriangleI(uint v1, uint v2, uint v3)
{
    indices.push_back(v1);
    indices.push_back(v2);
    indices.push_back(v3);

    state = DIRTY;
}

inline void VertexBuffer::addTriangleI(uint offset)
{
    indices.push_back(offset);
    indices.push_back(offset + 1);
    indices.push_back(offset + 2);

    state = DIRTY;
}

inline void VertexBuffer::addQuadI(uint v1, uint v2, uint v3, uint v4)
{
    addTriangleI(v1, v2, v3);
    addTriangleI(v1, v3, v4);
}

inline void VertexBuffer::addQuadI(uint offset)
{
    addTriangleI(offset, offset + 1, offset + 2);
    addTriangleI(offset, offset + 2, offset + 3);
}

template <typename T>
inline void VertexBuffer::pushVector(const T& v)
{
    pushFloats(glm::value_ptr(v), numFloats(v));
}

template <typename T>
inline void VertexBuffer::pushVertex(const T& vertex)
{
    pushFloats((float*) &vertex, numFloats(vertex));
}

inline void VertexBuffer::pushVertices(const float* floats, uint fcount)
{
    pushFloats(floats, fcount);
}

template <typename T>
inline void VertexBuffer::pushVertices(const T* vertices, uint vcount)
{
    pushFloats((float*) vertices, vcount * numFloats(vertices[0]));
}

template <typename T>
inline void VertexBuffer::push(const T* vertices, uint vcount, const uint* indices, uint icount)
{
    push((float*) vertices, vcount * numFloats(vertices[0]),
         indices, icount);
}

template <typename T>
inline void VertexBuffer::pushItem(const T* vertices, uint vcount, const uint* indices, uint icount)
{
    pushItem((float*) vertices, vcount * numFloats(vertices[0]), indices, icount);
}

} // namespace

#endif
