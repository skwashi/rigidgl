/**
 * @author Jonas Ransjö
 */

#ifndef TEXTUREUTILS_H
#define TEXTUREUTILS_H

#include "gltexture.h"
#include "cubemap.h"

namespace rgl
{

enum CubeMapLayout {
    CONSTANT,
    STRIP,
    DDS,
    HORIZONTAL,
    VERTICAL
};

GLTexture* createTexture(const std::string& filename, bool mipmap = false);

CubeMap* createCubeMap(const std::string& negX, const std::string& posX,
                       const std::string& negY, const std::string& posY,
                       const std::string& negZ, const std::string& posZ, bool mipmap = false);

CubeMap* createCubeMapD(const std::string& dir, bool mipmap = false);

CubeMap* createCubeMap(const std::string* filenames, bool mipmap = false);

CubeMap* createCubeMap(const CubeMapLayout& layout,
                       const std::string& filename, bool mipmap = false);
}

#endif
