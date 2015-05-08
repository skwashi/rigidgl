/**
 * @author Jonas Ransjö
 */


#include "rtypes.h"
#include "vertexbuffer.h"
#include "vertexarray.h"

using namespace rgl;

VertexBuffer::VertexBuffer(const std::vector<VertexAttrib>& attribs,
    GLenum usage)
{
    init(attribs, usage);
}

VertexBuffer::VertexBuffer(const VertexAttrib* attribs, uint count,
                           GLenum usage)
{
    init(attribs, count, usage);
}

void VertexBuffer::init(const std::vector<VertexAttrib>& attribs, GLenum usage)
{
    vertexArray.init(attribs);
    setUsage(usage);
    vsize = vertexArray.getNumComponents();
}

void VertexBuffer::init(const VertexAttrib* attribs, uint count,
                        GLenum usage)
{
    vertexArray.init(attribs, count);
    setUsage(usage);
    vsize = vertexArray.getNumComponents();
}

void VertexBuffer::useIndices(bool flag)
{
    vertexArray.useIndices(flag);
    usingIndices = flag;
}

void VertexBuffer::setUsage(GLenum usage)
{
    this->usage = usage;
}

VertexBuffer::Item VertexBuffer::getItem(uint index) const
{
    return items[index];
}

uint VertexBuffer::getVertexCount() const
{
    return vertices.size() / vsize;
}

uint VertexBuffer::getIndexCount() const
{
    return indices.size();
}

uint VertexBuffer::getItemCount() const
{
    return items.size();
}

bool VertexBuffer::isUsingIndices() const
{
    return usingIndices;
}

bool VertexBuffer::isInited() const
{
    return vertexArray.isInited();
}

void VertexBuffer::reserve(uint vcount)
{
    reserveF(vcount * vsize);
}

void VertexBuffer::reserve(uint vcount, uint icount)
{
    reserveF(vcount * vsize, icount);
}

void VertexBuffer::reserveF(uint fcount)
{
    vertices.reserve(vertices.size() + fcount);
}

void VertexBuffer::reserveF(uint fcount, uint icount)
{
    vertices.reserve(vertices.size() + fcount);
    indices.reserve(indices.size() + icount);
}

void VertexBuffer::clearVertices()
{
    vertices.clear();
}

void VertexBuffer::clearIndices()
{
    indices.clear();
}

void VertexBuffer::clear()
{
    clearVertices();
    clearIndices();
}

void VertexBuffer::push(const float* floats, uint fcount, const uint* indices, uint icount)
{
    uint vstart = getVertexCount();
    uint istart = getIndexCount();
    pushFloats(floats, fcount);
    pushIndices(indices, icount);

    for (int i = istart, iend = istart + icount; i < iend; i++)
        this->indices[i] += vstart;

    state = DIRTY;
}

uint VertexBuffer::pushItem(const float* floats, uint fcount, const uint* indices, uint icount)
{
    uint vstart = getVertexCount();
    uint istart = getIndexCount();
    pushFloats(floats, fcount);
    pushIndices(indices, icount);

    for (int i = istart, iend = istart + icount; i < iend; i++)
        this->indices[i] += vstart;

    Item item = {vstart, fcount / vsize, istart, icount};
    items.push_back(item);
    state = DIRTY;

    return items.size() - 1;
}

void VertexBuffer::bind() const
{
    vertexArray.bind();
}

void VertexBuffer::bufferData(GLenum usage)
{
    state = BUFFERING;
    vertexArray.bufferData(&vertices[0], vertices.size(),
                           &indices[0], indices.size(), usage);
    state = CLEAN;
}

void VertexBuffer::render(GLenum mode) {
    vertexArray.bind();

    if (state == DIRTY)
        bufferData(usage);

    if (vertexArray.getVertexCount() > 0)
        vertexArray.drawElements(mode);
}

void VertexBuffer::renderItem(uint index, GLenum mode)
{
    vertexArray.bind();

    if (state == DIRTY)
        bufferData(usage);

    Item item = items[index];
    vertexArray.drawElements(item.istart, item.icount, mode);
}

void VertexBuffer::renderItems(uint index, uint count, GLenum mode)
{
    vertexArray.bind();
    if (state == DIRTY)
        bufferData(usage);

    Item firstItem = items[index];
    Item lastItem = items[index + count];
    uint istart = firstItem.istart;
    uint icount = lastItem.istart + lastItem.icount - istart;

    vertexArray.drawElements(istart, icount, mode);
}
