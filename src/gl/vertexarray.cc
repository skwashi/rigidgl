#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vertexattrib.h"
#include "vertexarray.h"

using namespace rgl;

VertexArray::VertexArray(const std::vector<VertexAttrib>& _attribs)
{
    usingIndices = true;
    attribs = _attribs;
    initBuffers();
    glGenBuffers(1, &iboId);
}

VertexArray::VertexArray(const VertexAttrib* _attribs, int count)
{
    usingIndices = true;
    attribs.reserve(count);
    for (int i = 0; i < count; i++) {
        attribs.push_back(_attribs[i]);
    }
    initBuffers();
    glGenBuffers(1, &iboId);
}

VertexArray::VertexArray(int vertexCapacity, int indexCapacity,
                         const std::vector<VertexAttrib>& _attribs) : VertexArray(_attribs)
{
    vertices.reserve(vertexCapacity * numComponents);
    indices.reserve(indexCapacity);
}

VertexArray::VertexArray(int vertexCapacity, int indexCapacity,
                         const VertexAttrib* _attribs, int count) : VertexArray(_attribs, count)
{
    vertices.reserve(vertexCapacity * numComponents);
    indices.reserve(indexCapacity);
}

VertexArray::VertexArray(int vertexCapacity,
                         const std::vector<VertexAttrib>& _attribs)
{
    usingIndices = false;
    attribs = _attribs;
    initBuffers();
    vertices.reserve(vertexCapacity * numComponents);
}

VertexArray::VertexArray(int vertexCapacity,
                         const VertexAttrib* _attribs, int count)
{
    usingIndices = false;
    attribs.reserve(count);
    for (int i = 0; i < count; i++) {
        attribs.push_back(_attribs[i]);
    }
    initBuffers();
    vertices.reserve(vertexCapacity * numComponents);
}

void VertexArray::bind()
{
    glBindVertexArray(vaoId);
    for (VertexAttrib& attrib : attribs) {
        glEnableVertexAttribArray(attrib.location);
    }
    if (usingIndices) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    }
}

void VertexArray::unbind()
{
    // for (VertexAttrib &attrib : attribs) {
    //     glDisableVertexAttribArray(attrib.location);
    // }

    glBindVertexArray(0);
    if (usingIndices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexArray::bufferVertexData(GLenum usage)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 &vertices[0], usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::bufferVertexData(const float* data, unsigned int count, GLenum usage)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void VertexArray::bufferIndexData(GLenum usage)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], usage);
}

void VertexArray::bufferIndexData(const int* data, unsigned int count, GLenum usage)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage);
}

void VertexArray::destroy()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vboId);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vaoId);

    if (usingIndices) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &iboId);
    }

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
