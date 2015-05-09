/**
 * @author Jonas Ransjö
 */


#include "rtypes.h"
#include "vertexbuffer.h"
#include "vertexattrib.h"
#include "vertexarray.h"

using namespace rgl;

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

const VertexAttribs& VertexBuffer::getAttribs() const
{
    return vertexArray.attribs;
}

uint VertexBuffer::getItemCount() const
{
    return items.size();
}

uint VertexBuffer::getVertexSize() const
{
    return vertexArray.getNumComponents();
}

bool VertexBuffer::isUsingIndices() const
{
    return usingIndices;
}

bool VertexBuffer::isInited() const
{
    return vertexArray.isInited();
}

void VertexBuffer::bind() const
{
    vertexArray.bind();
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
