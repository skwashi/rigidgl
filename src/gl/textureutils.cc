/**
 * @author Jonas Ransjö
 */

#include <GL/glew.h>
#include <FreeImage.h>

#include "rtypes.h"
#include "../rutils.h"
#include "gltexture.h"
#include "cubemap.h"
#include "textureutils.h"

namespace rgl
{

GLTexture* createTexture(const std::string& filename, bool mipmap)
{
    const char* fname = filename.c_str();
    uint width, height, bpp;
    byte* data;

    if (rutils::loadImage(fname, width, height, bpp, &data)) {
        GLTexture* tex = new GLTexture(bpp == 24 ? GL_RGB : GL_RGBA, bpp == 24 ? GL_BGR : GL_BGRA, width, height, data, mipmap);
        free(data);
        return tex;
    } else {
        return NULL;
    }
}

CubeMap* createCubeMap(const std::string& negX, const std::string& posX,
                       const std::string& negY, const std::string& posY,
                       const std::string& negZ, const std::string& posZ, bool mipmap)
{
    const std::string filenames[6] = {negX, posX, negY, posY, negZ, posZ};
    return createCubeMap(filenames, mipmap);
}

CubeMap* createCubeMap(const std::string* filenames, bool mipmap)
{
    const char* fname = filenames[0].c_str();
    FIBITMAP* bitmap = rutils::loadImage(fname);

    if (!bitmap)
        return NULL;

    uint width = FreeImage_GetWidth(bitmap);
    uint height = FreeImage_GetHeight(bitmap);
    uint bpp = FreeImage_GetBPP(bitmap);

    uint size = width;

    if (height != size) {
        std::cerr << "Wrong dimensions in cubemap files." << std::endl;
        FreeImage_Unload(bitmap);
        return NULL;
    }

    CubeMap* cubeMap = new CubeMap(bpp == 24 ? GL_RGB : GL_RGBA,
                                   size);

    byte* bits = FreeImage_GetBits(bitmap);
    cubeMap->bufferData(CubeMap::SIDES[0], bpp == 24 ? GL_BGR : GL_BGRA, bits);

    for (int i = 1; i < 6; i++) {
        fname = filenames[i].c_str();
        bitmap = rutils::loadImage(fname);
        if (!bitmap)
            return NULL;
        width = FreeImage_GetWidth(bitmap);
        height = FreeImage_GetHeight(bitmap);
        if (width != size || height != size) {
            std::cerr << "Wrong dimensions in cubemap files." << std::endl;
            FreeImage_Unload(bitmap);
            return NULL;
        }
        bpp = FreeImage_GetBPP(bitmap);
        bits = FreeImage_GetBits(bitmap);
        cubeMap->bufferData(CubeMap::SIDES[i], bpp == 24 ? GL_BGR : GL_BGRA, bits);

        if (mipmap) {
            cubeMap->generateMipmap();
            cubeMap->setFilter(CubeMap::DEFAULT_MIN_FILTER_MIPMAP,
                               CubeMap::DEFAULT_MAG_FILTER);
        }
        FreeImage_Unload(bitmap);
    }

    return cubeMap;
}


CubeMap* createCubeMap(const CubeMapLayout& layout,
                       const std::string& filename, bool mipmap)
{
    return NULL;
}

}


