/**
 * @file framebuffer.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-15
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

#include "gltexture.h"

namespace rgl {

class FrameBuffer
{
public:

    struct Attachment
    {

    };

    FrameBuffer();
    ~FrameBuffer();

    GLuint getId() { return fboId; }

    void bind() { glBindFramebuffer(GL_FRAMEBUFFER, fboId); }
    void read() { glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId); }
    void write() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId); }

    void attachTexture(const GLTexture& texture, GLenum attachment = GL_COLOR_ATTACHMENT0, GLenum target = GL_FRAMEBUFFER);

    void detatchTextures();

protected:
    GLuint fboId = 0;
};

} // namespace

#endif
