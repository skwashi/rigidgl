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
#include "vertexarray.h"
#include "vertexattrib.h"
#include "vertexbuffer.h"

namespace rgl {

class TextBuffer
{
public:

    TextBuffer();
    ~TextBuffer();

protected:
    VertexBuffer vertexBuffer;
    ftgl::font_manager_t* fontManager;

private:

};

} // namespace

#endif
