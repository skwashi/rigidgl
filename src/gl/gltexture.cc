/**
 * @author Jonas Ransjö
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "gltexture.h"

using namespace rgl;

GLenum GLTexture::defaultFormat(GLint internalFormat)
{
    GLint f = internalFormat;

    if (f == GL_R8 || f == GL_R8_SNORM || f == GL_R16F || f == GL_R32F)
        return GL_RED;

    if (f == GL_R8UI || f == GL_R8I || f == GL_R16UI || f == GL_R16I
        || f == GL_R32UI || f == GL_R32I)
        return GL_RED_INTEGER;

    if (f == GL_RG8 || f == GL_RG8_SNORM || f == GL_RG16F || f == GL_RG32F)
        return GL_RG;

    if (f == GL_RG8UI || f == GL_RG8I || f == GL_RG16UI || f == GL_RG16I
        || f == GL_RG32UI || f == GL_RG32I)
        return GL_RG_INTEGER;

    if (f == GL_RGB8 || f == GL_SRGB8 || f == GL_RGB565
        || f == GL_RGB8_SNORM || f == GL_R11F_G11F_B10F
        || f == GL_RGB9_E5 || f == GL_RGB16F || f == GL_RGB32F)
        return GL_RGB;

    if (f == GL_RGB8UI || f == GL_RGB8I || f == GL_RGB16UI
        || f ==GL_RGB16I || f == GL_RGB32UI || f == GL_RGB32I)
        return GL_RGB_INTEGER;

    if (f == GL_RGBA || f == GL_RGBA8 || f == GL_SRGB8_ALPHA8
        || f == GL_RGBA8_SNORM || f == GL_RGB5_A1 || f == GL_RGBA4
        || f == GL_RGB10_A2 || f == GL_RGBA16F || f == GL_RGBA32F)
        return GL_RGBA;

    if (f == GL_RGBA8UI || f == GL_RGBA8I || f == GL_RGB10_A2UI
        || f == GL_RGBA16UI || f == GL_RGBA16I || f == GL_RGBA32I
        || f == GL_RGBA32UI)
        return GL_RGBA_INTEGER;

    if (f == GL_DEPTH_COMPONENT16 || f == GL_DEPTH_COMPONENT24
        || f == GL_DEPTH_COMPONENT32F || f == GL_DEPTH_COMPONENT32)
        return GL_DEPTH_COMPONENT;

    if (f == GL_DEPTH24_STENCIL8 || f == GL_DEPTH32F_STENCIL8)
        return GL_DEPTH_STENCIL;

    return f;
}

GLTexture::GLTexture(GLint internalFormat, GLsizei width, GLsizei height, GLint filter, GLint wrap)
{
    this->internalFormat = internalFormat;
    this->format = defaultFormat(internalFormat);
    this->width = width;
    this->height = height;
    glGenTextures(1, &textureId);
    bind();
    setPackUnpackAlignment();
    bufferData(NULL);
    setFilter(filter, filter);
    setWrap(wrap);
}

GLTexture::GLTexture(GLint internalFormat, GLenum format, GLenum type, GLsizei width, GLsizei height, const GLvoid* data, bool mipmap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT)
{
    this->internalFormat = internalFormat;
    this->format = format;
    this->type = type;
    this->width = width;
    this->height = height;
    glGenTextures(1, &textureId);
    bind();
    setPackUnpackAlignment();
    bufferData(format, type, data);
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

void GLTexture::resize(GLsizei width, GLsizei height)
{
    this->width = width;
    this->height = height;
    bind();
    bufferData(NULL);
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
    bufferData(format, type, data);
}

void GLTexture::bufferData(GLenum format, const GLvoid* data)
{
    bufferData(format, type, data);
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
