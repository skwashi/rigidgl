/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gltexture.h"
#include "cubemap.h"

using namespace rgl;

const GLenum CubeMap::SIDES[] = {
    NEG_X, POS_X,
    NEG_Y, POS_Y,
    NEG_Z, POS_Z
};

CubeMap::CubeMap(GLint internalFormat, GLsizei size, GLint filter, GLint wrap)
{
    TEXTURE_TYPE = GL_TEXTURE_CUBE_MAP;
    this->internalFormat = internalFormat;
    this->width = size;
    this->height = size;
    glGenTextures(1, &textureId);
    bind();
    setFilter(filter);
    setWrap(wrap);
}

CubeMap::CubeMap(GLint internalFormat, GLenum format, GLsizei size,
                 const GLvoid* negX, const GLvoid* posX,
                 const GLvoid* negY, const GLvoid* posY,
                 const GLvoid* negZ, const GLvoid* posZ,
                 bool mipmap, GLint minFilter, GLint magFilter, GLint wrap)
{
    TEXTURE_TYPE = GL_TEXTURE_CUBE_MAP;
    this->internalFormat = internalFormat;
    this->width = size;
    this->height = size;
    glGenTextures(1, &textureId);
    bind();
    setPackUnpackAlignment();
    bufferData(format, negX, posX, negY, posY, negZ, posZ);
    if (mipmap)
        generateMipmap();
    setFilter(minFilter, magFilter);
    setWrap(wrap);
}

CubeMap::~CubeMap()
{
    if (textureId != 0)
        glDeleteTextures(1, &textureId);
    textureId = 0;
}


void CubeMap::bufferData(GLenum side, const GLvoid* data)
{
    bufferData(side, DEFAULT_FORMAT, DEFAULT_TYPE, data);
}

void CubeMap::bufferData(GLenum side, GLenum format, const GLvoid* data)
{
    bufferData(side, format, DEFAULT_TYPE, data);
}

void CubeMap::bufferData(GLenum side, GLenum format, GLenum type, const GLvoid* data)
{
    setPackUnpackAlignment();
    glTexImage2D(side, 0, internalFormat, width, height, 0, format, type, data);
}

void CubeMap::bufferData(GLenum side, GLint x, GLint y, GLsizei width, GLsizei height, const GLvoid* data)
{
    bufferData(side, x, y, width, height, DEFAULT_FORMAT, DEFAULT_TYPE, data);
}

void CubeMap::bufferData(GLenum side, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, const GLvoid* data)
{
    bufferData(side, x, y, width, height, format, DEFAULT_TYPE, data);
}

void CubeMap::bufferData(GLenum side, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
{
    setPackUnpackAlignment();
    glTexSubImage2D(side, 0, x, y, width, height, format, type, data);
}

void CubeMap::bufferData(const GLvoid* negX, const GLvoid* posX,
                         const GLvoid* negY, const GLvoid* posY,
                         const GLvoid* negZ, const GLvoid* posZ)
{
    bufferData(DEFAULT_FORMAT, DEFAULT_TYPE,
               negX, posX, negY, posY, negZ, posZ);
}

void CubeMap::bufferData(GLenum format,
                const GLvoid* negX, const GLvoid* posX,
                const GLvoid* negY, const GLvoid* posY,
                const GLvoid* negZ, const GLvoid* posZ)
{
    bufferData(format, DEFAULT_TYPE,
               negX, posX, negY, posY, negZ, posZ);
}

void CubeMap::bufferData(GLenum format, GLenum type,
                         const GLvoid* negX, const GLvoid* posX,
                         const GLvoid* negY, const GLvoid* posY,
                         const GLvoid* negZ, const GLvoid* posZ)
{
    bufferData(NEG_X, format, type, negX);
    bufferData(POS_X, format, type, posX);
    bufferData(NEG_Y, format, type, negY);
    bufferData(POS_Y, format, type, posY);
    bufferData(NEG_Z, format, type, negZ);
    bufferData(POS_Z, format, type, posZ);
}
