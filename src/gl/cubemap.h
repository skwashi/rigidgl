/**
 * @author Jonas Ransjö
 */

#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gltexture.h"

namespace rgl
{

class CubeMap : public GLTexture
{
public:
    static const GLenum NEG_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    static const GLenum POS_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    static const GLenum NEG_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    static const GLenum POS_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    static const GLenum NEG_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
    static const GLenum POS_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;

    static const int NUM_SIDES = 6;
    static const GLenum SIDES[6];

    static const GLint DEFAULT_WRAP_S = GL_CLAMP_TO_EDGE;
    static const GLint DEFAULT_WRAP_T = GL_CLAMP_TO_EDGE;
    static const GLint DEFAULT_WRAP_R = GL_CLAMP_TO_EDGE;
    static const GLint DEFAULT_WRAP = GL_CLAMP_TO_EDGE;

    CubeMap(GLsizei size)
        : CubeMap(DEFAULT_INTERNAL_FORMAT, size) {}

    CubeMap(GLsizei size, GLint filter, GLint wrap)
        : CubeMap(DEFAULT_INTERNAL_FORMAT, size, filter, wrap) {}

    CubeMap(GLint internalFormat, GLsizei size)
        : CubeMap(internalFormat, size, DEFAULT_FILTER, DEFAULT_WRAP) {}

    CubeMap(GLint internalFormat, GLsizei size, GLint filter, GLint wrap);

    CubeMap(GLsizei size,
            const GLvoid* negX, const GLvoid* posX,
            const GLvoid* negY, const GLvoid* posY,
            const GLvoid* negZ, const GLvoid* posZ,
            bool mipmap = false)
        : CubeMap(DEFAULT_INTERNAL_FORMAT, DEFAULT_FORMAT, size,
                  negX, posX, negY, posY, negZ, posZ,
                  mipmap, mipmap ? DEFAULT_MIN_FILTER_MIPMAP : DEFAULT_MIN_FILTER, DEFAULT_MAG_FILTER, DEFAULT_WRAP) {}

    CubeMap(GLenum format, GLsizei size,
            const GLvoid* negX, const GLvoid* posX,
            const GLvoid* negY, const GLvoid* posY,
            const GLvoid* negZ, const GLvoid* posZ,
            bool mipmap = false)
        : CubeMap(DEFAULT_INTERNAL_FORMAT, format, size,
                  negX, posX, negY, posY, negZ, posZ,
                  mipmap, mipmap ? DEFAULT_MIN_FILTER_MIPMAP : DEFAULT_MIN_FILTER, DEFAULT_MAG_FILTER, DEFAULT_WRAP) {}

    CubeMap(GLint internalFormat, GLenum format, GLsizei size,
            const GLvoid* negX, const GLvoid* posX,
            const GLvoid* negY, const GLvoid* posY,
            const GLvoid* negZ, const GLvoid* posZ,
            bool mipmap = false)
        : CubeMap(internalFormat, format, size,
                  negX, posX, negY, posY, negZ, posZ,
                  mipmap, mipmap ? DEFAULT_MIN_FILTER_MIPMAP : DEFAULT_MIN_FILTER, DEFAULT_MAG_FILTER, DEFAULT_WRAP) {}

    CubeMap(GLsizei size,
            const GLvoid* negX, const GLvoid* posX,
            const GLvoid* negY, const GLvoid* posY,
            const GLvoid* negZ, const GLvoid* posZ,
            bool mipmap, GLint minFilter, GLint magFilter, GLint wrap)
        : CubeMap(DEFAULT_INTERNAL_FORMAT, DEFAULT_FORMAT, size,
                  negX, posX, negY, posY, negZ, posZ,
                  mipmap, minFilter, magFilter, wrap) {}

    CubeMap(GLenum format, GLsizei size,
            const GLvoid* negX, const GLvoid* posX,
            const GLvoid* negY, const GLvoid* posY,
            const GLvoid* negZ, const GLvoid* posZ,
            bool mipmap, GLint minFilter, GLint magFilter, GLint wrap)
        : CubeMap(DEFAULT_INTERNAL_FORMAT, format, size,
                  negX, posX, negY, posY, negZ, posZ,
                  mipmap, minFilter, magFilter, wrap) {}

    CubeMap(GLint internalFormat, GLenum format, GLsizei size,
            const GLvoid* negX, const GLvoid* posX,
            const GLvoid* negY, const GLvoid* posY,
            const GLvoid* negZ, const GLvoid* posZ,
            bool mipmap, GLint minFilter, GLint magFilter, GLint wrap);

    ~CubeMap();

    void setWrap(GLint wrapS, GLint wrapT, GLint wrapR)
    {
        setParam(GL_TEXTURE_WRAP_S, wrapS);
        setParam(GL_TEXTURE_WRAP_T, wrapT);
        setParam(GL_TEXTURE_WRAP_R, wrapR);
    }

    virtual void setWrap(GLint wrap)
    {
        setWrap(wrap, wrap, wrap);
    }

    void set(bool mipmap, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint wrapR)
    {
        if (mipmap)
            generateMipmap();
        setFilter(minFilter, magFilter);
        setWrap(wrapS, wrapT, wrapR);
    }

    virtual void setDefaults(bool mipmap = false)
    {
        set(mipmap, mipmap ? DEFAULT_MIN_FILTER_MIPMAP : DEFAULT_MIN_FILTER, DEFAULT_MAG_FILTER, DEFAULT_WRAP_S, DEFAULT_WRAP_T, DEFAULT_WRAP_R);
    }

    virtual void bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    }

    void bufferData(GLenum side, const GLvoid* data);
    void bufferData(GLenum side, GLenum format, const GLvoid* data);
    void bufferData(GLenum side, GLenum format, GLenum type, const GLvoid* data);
    void bufferData(GLenum side, GLint x, GLint y, GLsizei width, GLsizei height, const GLvoid* data);
    void bufferData(GLenum side, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, const GLvoid* data);
    void bufferData(GLenum side, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data);

    void bufferData(const GLvoid* negX, const GLvoid* posX,
                    const GLvoid* negY, const GLvoid* posY,
                    const GLvoid* negZ, const GLvoid* posZ);
    void bufferData(GLenum format,
                    const GLvoid* negX, const GLvoid* posX,
                    const GLvoid* negY, const GLvoid* posY,
                    const GLvoid* negZ, const GLvoid* posZ);
    void bufferData(GLenum format, GLenum type,
                    const GLvoid* negX, const GLvoid* posX,
                    const GLvoid* negY, const GLvoid* posY,
                    const GLvoid* negZ, const GLvoid* posZ);


};

} // namespace

#endif
