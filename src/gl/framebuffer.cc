/**
 * @file framebuffer.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-15
 */

#include <GL/glew.h>
#include <cstdarg>
#include <vector>
#include <iostream>

#include "framebuffer.h"
#include "gltexture.h"

namespace rgl {


FrameBuffer* FrameBuffer::create(GLsizei width, GLsizei height,
                                 AttachmentPoint ap, GLint format)
{
    FrameBuffer* fb = new FrameBuffer();
    GLTexture* tex = new GLTexture(format, width, height);
    fb->attachTexture(tex, ap, true);
    return fb;
}

FrameBuffer* FrameBuffer::create(GLsizei width, GLsizei height,
                                 uint numAttachments, ...)
{
    FrameBuffer* fb = new FrameBuffer();
    AttachmentPoint ap;
    GLint format;
    va_list args;

    va_start (args, numAttachments);
    for (uint i = 0; i < numAttachments; i++) {
        ap = (AttachmentPoint) va_arg(args, GLenum);
        format = va_arg(args, GLint);
        fb->attachTexture(new GLTexture(format, width, height)
                          , ap, true);
    }
    va_end(args);

    return fb;
}

FrameBuffer::FrameBuffer(GLsizei width, GLsizei height)
{
    this->width = width;
    this->height = height;
    glGenFramebuffers(1, &fboId);
}

FrameBuffer::~FrameBuffer()
{
    if (fboId != 0)
        glDeleteFramebuffers(1, &fboId);
    fboId = 0;

    for (auto iter : attachments) {
        Attachment attachment = iter.second;
        if (attachment.owned)
            delete attachment.texture;
    }
}

void FrameBuffer::resize(GLsizei width, GLsizei height)
{
    for (auto& pair : attachments) {
        if (auto* tex = pair.second.texture) {
            tex->resize(width, height);
        }
    }
    this->width = width;
    this->height = height;
}

void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    sync();
    drawBuffers();
}

void FrameBuffer::bind(AttachmentPoint ap)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glViewport(0, 0, width, height);
    sync();
    glDrawBuffer(ap);
}

void FrameBuffer::read(AttachmentPoint ap)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    sync();
    glReadBuffer(ap);
}

void FrameBuffer::draw()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
    glViewport(0, 0, width, height);
    sync();
    drawBuffers();
}

void FrameBuffer::draw(AttachmentPoint ap)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
    glViewport(0, 0, width, height);
    sync();
    glDrawBuffer(ap);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture(int textureUnit, AttachmentPoint ap)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    GLTexture* texture = getTexture(ap);
    if (texture)
        texture->bind();
}

void FrameBuffer::addComponent(AttachmentPoint ap, GLint format,
                               GLint filter, GLint wrap)
{
    GLTexture* tex = new GLTexture(format, width, height, filter, wrap);
    attachTexture(tex, ap, true);
}

void FrameBuffer::addComponent(AttachmentPoint ap, GLint format, GLint type, GLint filter, GLint wrap)
{
    GLTexture* tex = new GLTexture(format, GLTexture::defaultFormat(format), type, width, height, filter, wrap);
    attachTexture(tex, ap, true);
}

void FrameBuffer::attachTexture(GLTexture* texture, AttachmentPoint ap, bool owned)
{
    auto& attachment = attachments[ap];
    if (attachment.owned)
        delete attachment.texture;
    attachment.ap = ap;
    attachment.texture = texture;
    attachment.owned = owned;
    attachment.synced = false;
    if (texture) {
        attachment.enabled = true;
        width = texture->getWidth();
        height = texture->getHeight();
    }
}

void FrameBuffer::detatchTexture(AttachmentPoint ap, bool destroy)
{
    auto iter = attachments.find(ap);
    if (iter != attachments.end()) {
        auto& a = iter->second;
        if (destroy)
            delete a.texture;
        a.texture = NULL;
        a.synced = false;
        a.enabled = false;
    }
}

void FrameBuffer::enable(AttachmentPoint ap)
{
    attachments[ap].enabled = true;
}

void FrameBuffer::disable(AttachmentPoint ap)
{
    attachments[ap].enabled = false;
}

void FrameBuffer::clear()
{
    for (auto& it : attachments) {
        auto& ap = it.second;
        if (ap.owned)
            delete ap.texture;
        ap.texture = NULL;
        ap.synced = false;
        ap.enabled = false;
    }
}

bool FrameBuffer::hasTexture(AttachmentPoint ap)
{
    return (getTexture(ap) != NULL);
}

GLTexture* FrameBuffer::getTexture(AttachmentPoint ap)
{
    auto it = attachments.find(ap);
    if (it != attachments.end())
        return it->second.texture;
    else
        return NULL;
}

void FrameBuffer::sync()
{
    for (auto& it : attachments) {
        auto& a = it.second;
        if (!a.synced && a.enabled) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, a.ap, GL_TEXTURE_2D, a.texture ? a.texture->getId() : 0, 0);
            a.synced = true;
        }
    }
}

void FrameBuffer::drawBuffers()
{
    std::vector<GLenum> colorAttachments;

    for (auto& it : attachments) {
        auto& a = it.second;
        if (a.ap >= GL_COLOR_ATTACHMENT0 &&
            a.ap <= GL_COLOR_ATTACHMENT7 &&
            a.enabled)
            colorAttachments.push_back(a.ap);
    }
    glDrawBuffers(colorAttachments.size(), &colorAttachments[0]);
}

} // namespace
