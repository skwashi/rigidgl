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

    virtual ~VertexBuffer();

    void useIndices(bool flag);
    void setUsage(GLenum usage);
    Item getItem(uint index) const;
    const VertexAttribs& getAttribs() const;
    uint getItemCount() const;
    uint getVertexSize() const;
    bool isUsingIndices() const;
    bool isInited() const;

    void bind() const;
    virtual void bufferData(GLenum usage) = 0;

    void render(GLenum mode = GL_TRIANGLES);
    void renderItem(uint index, GLenum mode = GL_TRIANGLES);
    void renderItems(uint index, uint count, GLenum mode = GL_TRIANGLES);

protected:
    VertexArray vertexArray;
    std::vector<Item> items;

    State state = CLEAN;
    GLenum usage;
    bool usingIndices;
};

template <typename V>
class VBuffer : public VertexBuffer
{
public:
    VBuffer(GLenum usage = GL_STATIC_DRAW);

    uint getVertexCount() const;
    uint getIndexCount() const;

    void bufferData(GLenum usage);

    void reserve(uint vcount);
    void reserve(uint vcount, uint icount);

    void increase(uint vcount);
    void increase(uint vcount, uint icount);

    void clearVertices();
    void clearIndices();
    void clear();

    void pushVertex(const V& vertex);
    void pushVertices(const V* vertices, uint count);
    void pushVertices(const std::vector<V>& vertices);

    void pushIndex(uint index);
    void pushIndices(const uint* indices, uint count);
    void pushIndices(const std::vector<uint>& indices);

    void push(const V* vertices, uint vcount,
              const uint* indices, uint icount, bool createItem = false);
    void push(const std::vector<V>& vertices,
              const std::vector<uint>& indices, bool createItem = false);

    uint pushItem(const V* vertices, uint vcount,
                  const uint* indices, uint icount);
    uint pushItem(const std::vector<V>& vertices,
                  const std::vector<uint>& indices);

    void addTriangleI(uint v1, uint v2, uint v3);
    void addTriangleI(uint offset);
    void addQuadI(uint v1, uint v2, uint v3, uint v4);
    void addQuadI(uint offset);

protected:
    std::vector<V> vertices;
    std::vector<uint> indices;
};

template <typename V>
inline VBuffer<V>::VBuffer(GLenum usage)
{
    V v;
    vertexArray.init(attribs(v));
}

template <typename V>
inline uint VBuffer<V>::getVertexCount() const
{
    return vertices.size();
}

template <typename V>
inline uint VBuffer<V>::getIndexCount() const
{
    return indices.size();
}

template <typename V>
inline void VBuffer<V>::bufferData(GLenum usage)
{
    state = BUFFERING;
    vertexArray.bufferData(vertices, indices, usage);
    state = CLEAN;
}

template <typename V>
inline void VBuffer<V>::reserve(uint vcount)
{
    vertices.reserve(vcount);
}

template <typename V>
inline void VBuffer<V>::reserve(uint vcount, uint icount)
{
    vertices.reserve(vcount);
    indices.reserve(icount);
}

template <typename V>
inline void VBuffer<V>::increase(uint vcount)
{
    vertices.reserve(vertices.size() + vcount);
}

template <typename V>
inline void VBuffer<V>::increase(uint vcount, uint icount)
{
    vertices.reserve(vertices.size() + vcount);
    indices.reserve(indices.size() + icount);
}

template <typename V>
inline void VBuffer<V>::clearVertices()
{
    vertices.clear();
}

template <typename V>
inline void VBuffer<V>::clearIndices()
{
    indices.clear();
}

template <typename V>
inline void VBuffer<V>::clear()
{
    clearVertices();
    clearIndices();
}

template <typename V>
inline void VBuffer<V>::pushVertex(const V& vertex)
{
    vertices.push_back(vertex);
    state = DIRTY;
}

template <typename V>
inline void VBuffer<V>::pushVertices(const V* _vertices, uint count)
{
    vertices.insert(vertices.end(), _vertices, _vertices + count);
    state = DIRTY;
}

template <typename V>
inline void VBuffer<V>::pushVertices(const std::vector<V>& _vertices)
{
    pushVertices(&_vertices[0], _vertices.size());
}

template <typename V>
inline void VBuffer<V>::pushIndex(uint index)
{
    indices.push_back(index);
    state = DIRTY;
}

template <typename V>
inline void VBuffer<V>::pushIndices(const uint* _indices, uint count)
{
    indices.insert(indices.end(), _indices, _indices + count);
    state = DIRTY;
}

template <typename V>
inline void VBuffer<V>::pushIndices(const std::vector<uint>& _indices)
{
    pushIndices(&_indices[0], _indices.size());
}

template <typename V>
inline void VBuffer<V>::push(const V* _vertices, uint vcount,
                             const uint* _indices, uint icount,
                             bool createItem)
{
    uint vstart = getVertexCount();
    uint istart = getIndexCount();
    pushVertices(_vertices, vcount);
    pushIndices(_indices, vcount);

    for (int i = istart, iend = istart + icount; i < iend; i++)
        indices[i] += vstart;

    if (createItem) {
        Item item = {vstart, vcount, istart, icount};
        items.push_back(item);
    }
}

template <typename V>
inline void VBuffer<V>::push(const std::vector<V>& _vertices,
                             const std::vector<uint>& _indices,
                             bool createItem)
{
    push(&_vertices[0], _vertices.size(),
         &_indices[0], _indices.size(), createItem);
}

template <typename V>
inline uint VBuffer<V>::pushItem(const V* _vertices, uint vcount,
              const uint* _indices, uint icount)
{
    push(_vertices, vcount, _indices, icount, true);
    return items.size() - 1;
}

template <typename V>
inline uint VBuffer<V>::pushItem(const std::vector<V>& vertices,
              const std::vector<uint>& indices)
{
    push(vertices, indices, true);
    return items.size() - 1;
}

template <typename V>
inline void VBuffer<V>::addTriangleI(uint v1, uint v2, uint v3)
{
    indices.push_back(v1);
    indices.push_back(v2);
    indices.push_back(v3);

    state = DIRTY;
}

template <typename V>
inline void VBuffer<V>::addTriangleI(uint offset)
{
    indices.push_back(offset);
    indices.push_back(offset + 1);
    indices.push_back(offset + 2);

    state = DIRTY;
}

template <typename V>
inline void VBuffer<V>::addQuadI(uint v1, uint v2, uint v3, uint v4)
{
    addTriangleI(v1, v2, v3);
    addTriangleI(v1, v3, v4);
}

template <typename V>
inline void VBuffer<V>::addQuadI(uint offset)
{
    addQuadI(offset, offset + 1, offset + 2, offset + 3);
}

} // namespace

#endif
