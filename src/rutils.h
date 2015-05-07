/**
 * @author Jonas Ransjö
 */

#ifndef RUTILS_H
#define RUTILS_H

#include <FreeImage.h>

#include <string>
#include "rtypes.h"
#include "gl/mesh.h"

namespace rutils
{

std::string readFile(const std::string& filename);
std::string readFile(const char* filename);

FIBITMAP* loadImage(const char* fname);

bool loadImage(const std::string& fname, uint& width, uint& height, uint& bpp, byte** data);
bool loadImage(const char* fname, uint& width, uint& height, uint& bpp, byte** data);

bool loadObj(const std::string& fname, rgl::Mesh& mesh);
bool loadObj(const char* fname, rgl::Mesh& mesh);


} // namespace

#endif
