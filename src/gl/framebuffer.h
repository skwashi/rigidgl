/**
 * @file framebuffer.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-15
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <map>

#include "gltexture.h"

namespace rgl {

class FrameBuffer
{
public:

    enum AttachmentPoint
    {
        COLOR0 = GL_COLOR_ATTACHMENT0,
        COLOR1 = GL_COLOR_ATTACHMENT1,
        COLOR2 = GL_COLOR_ATTACHMENT2,
        COLOR3 = GL_COLOR_ATTACHMENT3,
        COLOR4 = GL_COLOR_ATTACHMENT4,
        COLOR5 = GL_COLOR_ATTACHMENT5,
        COLOR6 = GL_COLOR_ATTACHMENT6,
        COLOR7 = GL_COLOR_ATTACHMENT7,
        DEPTH = GL_DEPTH_ATTACHMENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
    };

    struct Attachment
    {
        AttachmentPoint ap = COLOR0;
        GLTexture* texture = NULL;
        bool synced = false;
        bool owned = false;
        bool enabled = false;
    };

    static FrameBuffer* create(GLsizei width, GLsizei height,
                               AttachmentPoint = COLOR0,
                               GLint format = GLTexture::DEFAULT_INTERNAL_FORMAT);

    static FrameBuffer* create(GLsizei width, GLsizei height,
                               uint numAttachments, ...);

    static void bindDefault() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    static void readDefault() { glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); }
    static void drawDefault() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); }

    FrameBuffer() : FrameBuffer(0, 0) {}
    FrameBuffer(GLsizei width, GLsizei height);
    ~FrameBuffer();

    GLuint getId() { return fboId; }
    GLsizei getWidth() { return width; }
    GLsizei getHeight() { return height; }

    void resize(GLsizei width, GLsizei height);

    void bind();
    void bind(AttachmentPoint ap);
    void read(AttachmentPoint ap = COLOR0);
    void draw();
    void draw(AttachmentPoint ap);
    void unbind();

    void bindTexture(int textureUnit = 0, AttachmentPoint ap = COLOR0);

    void addComponent(AttachmentPoint ap, GLint format)
    {
        addComponent(ap, format, GL_NEAREST, GL_REPEAT);
    }
    void addComponent(AttachmentPoint ap, GLint format, GLint filter, GLint wrap);
    void addComponent(AttachmentPoint ap, GLint format, GLint type)
    {
        addComponent(ap, format, type, GL_NEAREST, GL_REPEAT);
    }
    void addComponent(AttachmentPoint ap, GLint format, GLint type, GLint filter, GLint wrap);

    void attachTexture(GLTexture* texture, AttachmentPoint ap = COLOR0, bool owned = false);
    void detatchTexture(AttachmentPoint ap = COLOR0, bool destroy = false);
    void enable(AttachmentPoint ap);
    void disable(AttachmentPoint ap);

    void clear();
    bool hasTexture(AttachmentPoint = COLOR0);
    GLTexture* getTexture(AttachmentPoint = COLOR0);

protected:
    void sync();
    void drawBuffers();

    GLuint fboId = 0;
    std::map<AttachmentPoint, Attachment> attachments;
    GLsizei width = 0;
    GLsizei height = 0;
};

} // namespace

#endif
