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

    VertexArray() {}

    VertexArray(const std::vector<VertexAttrib>& attribs);

    VertexArray(const VertexAttrib* attribs, uint count);

    ~VertexArray();

    void init(const std::vector<VertexAttrib>& attribs);

    void init(const VertexAttrib* attribs, uint count);

    void bind() const;

    void unbind() const;

    void useIndices(bool flag) {
        usingIndices = flag;
    }

    void bufferVertexData(const float* data, unsigned int count, GLenum usage = GL_STATIC_DRAW);

    void bufferIndexData(const uint* data, unsigned int count, GLenum usage = GL_STATIC_DRAW);

    void bufferData(const float* vertexData, uint vcount,
                    const uint* indexData, uint icount,
                    GLenum usage = GL_STATIC_DRAW)
    {
        bufferVertexData(vertexData, vcount, usage);
        bufferIndexData(indexData, icount, usage);
    }

    void drawArrays(GLenum mode = GL_TRIANGLES) const
    {
        glDrawArrays(mode, 0, getVertexCount());
    }

    void drawArrays(uint count, GLenum mode) const
    {
        glDrawArrays(mode, 0, count);
    }

    void drawArrays(uint start, uint count, GLenum mode) const
    {
        glDrawArrays(mode, start, count);
    }

    void drawElements(GLenum mode = GL_TRIANGLES) const
    {
        glDrawElements(mode, getIndexCount(), GL_UNSIGNED_INT, (void*) 0);
    }

    void drawElements(uint count, GLenum mode) const
    {
        glDrawElements(mode, getIndexCount(), GL_UNSIGNED_INT, (void*) 0);
    }

    void drawElements(uint start, uint count, GLenum mode) const
    {
        glDrawElements(mode, count, GL_UNSIGNED_INT, (void*) (start * sizeof(GLuint)));
    }

    int getVertexCount() const
    {
        return vcount;
    }

    int getIndexCount() const
    {
        return icount;
    }

    int getNumComponents() const
    {
        return numComponents;
    }

    int getStride() const
    {
        return stride;
    }

    bool isUsingIndices() const
    {
        return usingIndices;
    }

    bool isInited() const
    {
        return vaoId != 0;
    }

    void destroy();

private:
    int numComponents;
    int stride;
    GLuint vaoId = 0;
    GLuint vboId = 0;
    GLuint iboId = 0;
    bool usingIndices = true;
    uint vcount = 0;
    uint icount = 0;

    void initBuffers();
};

}

#endif
