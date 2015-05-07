/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cstring>
#include <vector>
#include <FreeImage.h>

#include "rtypes.h"
#include "gl/mesh.h"

namespace rutils
{

std::string readFile(const char* filename)
{
    std::ifstream ifs(filename);
    std::string str((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>());
    ifs.close();
    return str;
}

std::string readFile(const std::string& filename)
{
    return readFile(filename.c_str());
}

FIBITMAP* loadImage(const char* fname)
{
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fname, 0);

    if (format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(fname);

    if (format != FIF_UNKNOWN && FreeImage_FIFSupportsReading(format)) {
        std::cout << "Loaded image file " << fname << std::endl;
        return FreeImage_Load(format, fname);
    }
    else {
        std::cerr << "Unable to load image file " << fname << std::endl;
        return NULL;
    }
}

bool loadImage(const std::string& fname, uint& width, uint& height, uint& bpp, byte** data)
{
    return loadImage(fname.c_str(), width, height, bpp, data);
}

bool loadImage(const char* fname, uint& width, uint& height, uint& bpp, byte** data)
{
    FIBITMAP* bitmap = loadImage(fname);

    if (bitmap) {
        width = FreeImage_GetWidth(bitmap);
        height = FreeImage_GetHeight(bitmap);
        bpp = FreeImage_GetBPP(bitmap);

        byte* bits = (byte*) FreeImage_GetBits(bitmap);
        *data = (byte*) malloc(width * height * bpp);
        memcpy(*data, bits, width * height * bpp / 8);

        FreeImage_Unload(bitmap);
        return true;
    }
    else {
        std::cerr << "Unable to load image file " << fname << std::endl;
        return false;
    }

}

bool loadObj(const char* fname, rgl::Mesh& mesh)
{
    std::ifstream file(fname);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << fname << std::endl;
        return false;
    }

    bool textured = false;

    std::cout << "Loading mesh from file " << fname << std::endl;
    mesh.clear();
    std::string line;
    glm::vec3 vertex;
    glm::vec2 texCoord;
    std::vector<int> faceVs;
    std::vector<int> faceTs;
    while(!file.eof()) {
        std::getline(file, line);
        if (line.substr(0, 2) == "v ") {
            std::istringstream vertexLine(line.substr(2));
            vertexLine >> vertex.x;
            vertexLine >> vertex.y;
            vertexLine >> vertex.z;
            mesh.addVertex(vertex);
        } else if (line.substr(0, 3) == "vt ") {
            std::istringstream texCoordLine(line.substr(3));
            texCoordLine >> texCoord.s;
            texCoordLine >> texCoord.t;
        } else if (line.substr(0, 2) == "f ") {
            faceVs.clear();
            faceTs.clear();
            std::istringstream faceLine(line.substr(2));
            while(faceLine) {
                std::string faceVertex;
                faceLine >> faceVertex;
                size_t loc = faceVertex.find('/');
                int fv = atoi(faceVertex.substr(0, loc).c_str());
                if (fv != 0) {
                    faceVs.push_back(fv - 1);
                }
                if (loc != std::string::npos) {
                    std::string rest = faceVertex.substr(loc + 1);
                    int ft = atoi(rest.substr(0, rest.find('/')).c_str());
                    if (ft != 0) {
                        faceTs.push_back(ft - 1);
                    }
                }
            }
            int count = faceVs.size();
            if (!faceTs.empty()) {
                textured = true;
                mesh.addFace(rgl::Face(count, &faceVs[0], &faceTs[0]));
            } else {
                mesh.addFace(rgl::Face(count, &faceVs[0]));
            }

        } else {
        }
    }

    if (textured) {
        mesh.useTextures();
    }

    mesh.computeFaceNormals();
    mesh.computeVertexNormals();
    return true;
}

bool loadObj(const std::string& fname, rgl::Mesh& mesh)
{
    return loadObj(fname.c_str(), mesh);
}

} // namespace
