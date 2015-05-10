/**
 * @author Jonas Ransjö
 */

#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

#include <vector>
#include <GL/glew.h>


#include "font-manager.h"
#include "markup.h"

#include "rtypes.h"
#include "vertexbuffer.h"
#include "vertex.h"

namespace rgl {

class TextBuffer
{
public:

    TextBuffer(GLenum usage = GL_STREAM_DRAW);
    ~TextBuffer();

    void clear();
    void addChar(wchar_t c);
    void render();

protected:
    VBuffer<Vertex3tc> vertexBuffer;
    ftgl::font_manager_t* fontManager;

    GLenum usage;

private:

};

} // namespace

#endif
