/**
 * @author Jonas Ransjö
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gltexture.h"

using namespace rgl;

GLTexture::GLTexture(GLint internalFormat, GLsizei width, GLsizei height, GLint filter, GLint wrap)
{
    this->internalFormat = internalFormat;
    this->width = width;
    this->height = height;
    glGenTextures(1, &textureId);
    bind();
    setFilter(filter, filter);
    setWrap(wrap);
}

GLTexture::GLTexture(GLint internalFormat, GLenum format, GLsizei width, GLsizei height, const GLvoid* data, bool mipmap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT)
{
    this->internalFormat = internalFormat;
    this->width = width;
    this->height = height;
    glGenTextures(1, &textureId);
    bind();
    setPackUnpackAlignment();
    bufferData(format, data);
    if (mipmap)
        generateMipmap();
    setFilter(minFilter, magFilter);
    setWrap(wrapS, wrapT);
}

GLTexture::~GLTexture()
{
    if (textureId != 0)
        glDeleteTextures(1, &textureId);
    textureId = 0;
}

void GLTexture::setInternalFormat(GLint internalFormat)
{
    this->internalFormat = internalFormat;
}

void GLTexture::setParam(GLenum pname, GLint param)
{
    glTexParameteri(TEXTURE_TYPE, pname, param);
}

void GLTexture::setFilter(GLint minFilter, GLint magFilter)
{
    setParam(GL_TEXTURE_MIN_FILTER, minFilter);
    setParam(GL_TEXTURE_MAG_FILTER, magFilter);
}

void GLTexture::setFilter(GLint filter)
{
    setFilter(filter, filter);
}

void GLTexture::setWrap(GLint wrapS, GLint wrapT)
{
    setParam(GL_TEXTURE_WRAP_S, wrapS);
    setParam(GL_TEXTURE_WRAP_T, wrapT);
}

void GLTexture::setWrap(GLint wrap)
{
    setWrap(wrap, wrap);
}

void GLTexture::setDefaults(bool mipmap)
{
    set(mipmap, mipmap ? DEFAULT_MIN_FILTER_MIPMAP : DEFAULT_MIN_FILTER, DEFAULT_MAG_FILTER, DEFAULT_WRAP_S, DEFAULT_WRAP_T);
}

void GLTexture::set(bool mipmap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT)
{
    if (mipmap)
        generateMipmap();
    setFilter(minFilter, magFilter);
    setWrap(wrapS, wrapT);
}

void GLTexture::setPackUnpackAlignment()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

void GLTexture::generateMipmap()
{
    glGenerateMipmap(TEXTURE_TYPE);
}

void GLTexture::bind()
{
    glBindTexture(TEXTURE_TYPE, textureId);
}

void GLTexture::bufferData(const GLvoid* data)
{
    bufferData(DEFAULT_FORMAT, DEFAULT_TYPE, data);
}

void GLTexture::bufferData(GLenum format, const GLvoid* data)
{
    bufferData(format, DEFAULT_TYPE, data);
}

void GLTexture::bufferData(GLenum format, GLenum type, const GLvoid* data)
{
    setPackUnpackAlignment();
    glTexImage2D(TEXTURE_TYPE, 0, internalFormat, width, height, 0, format, type, data);
}

void GLTexture::bufferData(GLint x, GLint y, GLsizei width, GLsizei height, const GLvoid* data)
{
    bufferData(x, y, width, height, DEFAULT_FORMAT, DEFAULT_TYPE, data);
}

void GLTexture::bufferData(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, const GLvoid* data)
{
    bufferData(x, y, width, height, format, DEFAULT_TYPE, data);
}

void GLTexture::bufferData(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data)
{
    setPackUnpackAlignment();
    glTexSubImage2D(TEXTURE_TYPE, 0, x, y, width, height, format, type, data);
}
