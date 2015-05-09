#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vertexattrib.h"
#include "vertexarray.h"

using namespace rgl;

VertexArray::VertexArray(const std::vector<VertexAttrib>& _attribs)
{
    usingIndices = true;
    init(_attribs);
}

VertexArray::VertexArray(const VertexAttrib* _attribs, uint count)
{
    usingIndices = true;
    init(_attribs, count);
}

VertexArray::~VertexArray()
{
    destroy();
}

void VertexArray::init(const std::vector<VertexAttrib>& _attribs)
{
    attribs = _attribs;
    initBuffers();
    if (usingIndices)
        glGenBuffers(1, &iboId);
}

void VertexArray::init(const VertexAttrib* _attribs, uint count)
{
    attribs.reserve(count);
    for (uint i = 0; i < count; i++) {
        attribs.push_back(_attribs[i]);
    }
    initBuffers();
    if (usingIndices)
        glGenBuffers(1, &iboId);
}

void VertexArray::bind() const
{
    glBindVertexArray(vaoId);
    for (const VertexAttrib& attrib : attribs) {
        glEnableVertexAttribArray(attrib.location);
    }
    if (usingIndices) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    }
}

void VertexArray::unbind() const
{
    // for (VertexAttrib &attrib : attribs) {
    //     glDisableVertexAttribArray(attrib.location);
    // }

    glBindVertexArray(0);
    if (usingIndices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArray::bufferVertexData(const float* data, uint count, GLenum usage)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, usage);

    vcount = count / numComponents;
}

void VertexArray::bufferIndexData(const uint* data, uint count, GLenum usage)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage);

    icount = count;
}

void VertexArray::bufferIndexData(const std::vector<uint>& indices, GLenum usage)
{
    bufferIndexData(&indices[0], indices.size());
}

void VertexArray::destroy()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (vboId != 0)
        glDeleteBuffers(1, &vboId);

    glBindVertexArray(0);
    if (vaoId != 0)
        glDeleteVertexArrays(1, &vaoId);

    if (usingIndices && iboId != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &iboId);
    }

    vboId = 0;
    iboId = 0;
    vaoId = 0;
}

void VertexArray::initBuffers()
{
    numComponents = 0;
    for (VertexAttrib &attrib : attribs) {
        numComponents += attrib.numComponents;
    }

    stride = numComponents * sizeof(float);

    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    intptr_t offset = 0;
    for (VertexAttrib &attrib : attribs) {
        glEnableVertexAttribArray(attrib.location);
        glVertexAttribPointer(attrib.location, attrib.numComponents,
                              GL_FLOAT, GL_FALSE, stride, (void*) offset);
        offset += attrib.numComponents * sizeof(float);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
