/**
 * @author Jonas Ransjö
 */

#ifndef RGL_H
#define RGL_H

#include <glm/glm.hpp>

#include "gltexture.h"
#include "cubemap.h"
#include "mesh.h"
#include "pipeline.h"
#include "shaderprogram.h"
#include "shaderprograms.h"
#include "framebuffer.h"
#include "textureutils.h"
#include "vertexarray.h"
#include "vertexattrib.h"
#include "vertex.h"
#include "vertexbuffer.h"
#include "glutils.h"
#include "renderable.h"

namespace rgl
{
typedef glm::vec4 Color;
typedef glm::vec2 TexCoord;
}


#endif
