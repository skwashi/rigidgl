#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vertexattrib.h"

namespace rgl {

class VertexArray
{
public:
    std::vector<VertexAttrib> attribs;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    VertexArray(int vertexCapacity, int indexCapacity,
                const std::vector<VertexAttrib>& _attribs);

    VertexArray(int vertexCapacity,
                const std::vector<VertexAttrib>& _attribs);

    VertexArray(int vertexCapacity, int indexCapacity,
                const VertexAttrib* _attribs, int count);

    VertexArray(int vertexCapacity,
                const VertexAttrib* _attribs, int count);

    void bind();
    void unbind();
    void bufferVertexData(GLenum usage);
    void bufferVertexData(const float* data, unsigned int count, GLenum usage);
    void bufferIndexData(GLenum usage);
    void bufferIndexData(const int* data, unsigned int count, GLenum usage);
    void bufferData(GLenum usage)
    {
        bufferVertexData(usage);
        if (usingIndices)
            bufferIndexData(usage);
    }

    void draw(GLenum mode)
    {
        glDrawArrays(mode, 0, getVertexCount());
    }

    void draw(GLenum mode, unsigned int count)
    {
        glDrawArrays(mode, 0, count);
    }

    void drawElements(GLenum mode)
    {
        glDrawElements(mode, getIndexCount(), GL_UNSIGNED_INT, (void*) 0);
    }

    void drawElements(GLenum mode, unsigned int count)
    {
        glDrawElements(mode, count, GL_UNSIGNED_INT, (void*) 0);
    }

    void clearVertices()
    {
        vertices.clear();
    }

    void clearIndices()
    {
        indices.clear();
    }

    void clear()
    {
        clearVertices();
        if (usingIndices)
            clearIndices();
    }

    int getVertexCount() const
    {
        return vertices.size() / numComponents;
    }

    int getIndexCount() const
    {
        return indices.size();
    }

    int getNumComponents() const
    {
        return numComponents;
    }

    int getStride() const
    {
        return stride;
    }

    bool isUsingIndices()
    {
        return usingIndices;
    }

    void destroy();

private:
    int numComponents;
    int stride;
    GLuint vaoId;
    GLuint vboId;
    GLuint iboId;
    bool usingIndices;

    void initBuffers();
};

}

#endif
