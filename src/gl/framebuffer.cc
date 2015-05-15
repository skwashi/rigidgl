/**
 * @file framebuffer.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-15
 */

#include <GL/glew.h>

#include "framebuffer.h"
#include "gltexture.h"

namespace rgl {

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &fboId);
}

FrameBuffer::~FrameBuffer()
{
    if (fboId != 0)
        glDeleteFramebuffers(1, &fboId);
    fboId = 0;
}

void FrameBuffer::attachTexture(const GLTexture& texture, GLenum attachment, GLenum target)
{
    glFramebufferTexture2D(target, attachment, GL_TEXTURE_2D, texture.getId(), 0);
}

void FrameBuffer::detatchTextures()
{

}

} // namespace
