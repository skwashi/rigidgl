/**
 * @author Jonas Ransjö
 */

#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace rgl
{

class GLTexture
{
public:
    static const GLint DEFAULT_MIN_FILTER = GL_LINEAR;
    static const GLint DEFAULT_MIN_FILTER_MIPMAP = GL_LINEAR_MIPMAP_LINEAR;
    static const GLint DEFAULT_MAG_FILTER = GL_LINEAR;
    static const GLint DEFAULT_FILTER = GL_LINEAR;
    static const GLint DEFAULT_WRAP_S = GL_REPEAT;
    static const GLint DEFAULT_WRAP_T = GL_REPEAT;
    static const GLint DEFAULT_WRAP = GL_REPEAT;
    static const GLint DEFAULT_INTERNAL_FORMAT = GL_RGBA8;
    static const GLenum DEFAULT_FORMAT = GL_RGBA;
    static const GLenum DEFAULT_TYPE = GL_UNSIGNED_BYTE;

    GLTexture() {}

    GLTexture(GLsizei width, GLsizei height)
        : GLTexture(width, height, DEFAULT_FILTER, DEFAULT_WRAP) {}

    GLTexture(GLint internalFormat, GLsizei width, GLsizei height)
        : GLTexture(internalFormat, width, height, DEFAULT_FILTER, DEFAULT_WRAP) {}

    GLTexture(GLsizei width, GLsizei height, GLint filter, GLint wrap)
        : GLTexture(DEFAULT_INTERNAL_FORMAT, width, height, DEFAULT_FILTER, DEFAULT_WRAP) {}

    GLTexture(GLint internalFormat, GLsizei width, GLsizei height, GLint filter, GLint wrap);

    GLTexture(GLsizei width, GLsizei height, const GLvoid* data, bool mipmap = false)
        : GLTexture(DEFAULT_INTERNAL_FORMAT, DEFAULT_FORMAT, width, height, data, mipmap, mipmap ? DEFAULT_MIN_FILTER_MIPMAP : DEFAULT_MIN_FILTER, DEFAULT_MAG_FILTER, DEFAULT_WRAP_S, DEFAULT_WRAP_T) {}

    GLTexture(GLint format, GLsizei width, GLsizei height, const GLvoid* data, bool mipmap = false)
        : GLTexture(DEFAULT_INTERNAL_FORMAT, format, width, height, data, mipmap, mipmap ? DEFAULT_MIN_FILTER_MIPMAP : DEFAULT_MIN_FILTER, DEFAULT_MAG_FILTER, DEFAULT_WRAP_S, DEFAULT_WRAP_T) {}

    GLTexture(GLint internalFormat, GLenum format, GLsizei width, GLsizei height, const GLvoid* data, bool mipmap = false)
        : GLTexture(internalFormat, format, width, height, data, mipmap, mipmap ? DEFAULT_MIN_FILTER_MIPMAP : DEFAULT_MIN_FILTER, DEFAULT_MAG_FILTER, DEFAULT_WRAP_S, DEFAULT_WRAP_T) {}

    GLTexture(GLsizei width, GLsizei height, const GLvoid* data, bool mipmap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT)
        : GLTexture(DEFAULT_FORMAT, width, height, data, mipmap, minFilter, magFilter, wrapS, wrapT) {}

    GLTexture(GLenum format, GLsizei width, GLsizei height, const GLvoid* data, bool mipmap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT)
        : GLTexture(DEFAULT_INTERNAL_FORMAT, format, width, height, data, mipmap, minFilter, magFilter, wrapS, wrapT) {}

    GLTexture(GLint internalFormat, GLenum format, GLsizei width, GLsizei height,const GLvoid* data, bool mipmap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT);

    ~GLTexture();

    void setInternalFormat(GLint internalFormat);
    virtual void setParam(GLenum pname, GLint param);
    void setFilter(GLint minFilter, GLint magFilter);
    void setFilter(GLint filter);
    void setWrap(GLint wrapS, GLint wrapT);
    virtual void setWrap(GLint wrap);
    virtual void setDefaults(bool mipmap = false);
    void set(bool mipmap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT);
    virtual void generateMipmap();

    virtual void bind();
    virtual void bufferData(const GLvoid* data);
    virtual void bufferData(GLenum format, const GLvoid* data);
    virtual void bufferData(GLenum format, GLenum type, const GLvoid* data);
    virtual void bufferData(GLint x, GLint y, GLsizei width, GLsizei height, const GLvoid* data);
    virtual void bufferData(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, const GLvoid* data);
    virtual void bufferData(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data);

protected:
    GLenum TEXTURE_TYPE = GL_TEXTURE_2D;
    GLuint textureId = 0;
    GLsizei width;
    GLsizei height;
    GLint internalFormat = DEFAULT_INTERNAL_FORMAT;

    void setPackUnpackAlignment();
};

} //namespace


#endif
