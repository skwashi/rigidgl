/**
 * @author Jonas Ransjö
 */

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "rgl.h"
#include "vertexattrib.h"
#include "vertexarray.h"

namespace rgl {

struct Face
{
    int count;
    int* vs;

    Face(int _count, const int* _vs)
    {
        count = _count;
        vs = new int[count];
        for (int i = 0; i < count; i++)
            vs[i] = _vs[i];
    }

    Face(const Face& face) : Face(face.count, face.vs) {}

    ~Face()
    {
        delete[] vs;
    }
};

struct TexFace : Face
{
    int* ts;

    TexFace(int _count, const int* _vs, const int* _ts) : Face(_count, _vs)
    {
        ts = new int[count];
        for (int i = 0; i < count; i++)
            ts[i] = _ts[i];
    }

    TexFace(const TexFace& face) : TexFace(face.count, face.vs, face.ts) {}

    ~TexFace()
    {
        delete[] vs;
        delete[] ts;
    }
};


class Mesh
{
public:
    Mesh() {}

    void addVertex(const glm::vec3& v);
    void addVertices(const glm::vec3* vs, int count);
    void addNormal(const glm::vec3& n);
    void addNormals(const glm::vec3* ns, int count);
    void addTexCoord(const TexCoord& n);
    void addTexCoords(const TexCoord* ns, int count);
    void addColor(const Color& n);
    void addColors(const Color* ns, int count);
    void addFace(const Face& face);
    void addFaces(const Face* faces, int count);
    void computeFaceNormals();
    void computeVertexNormals();

    void useTextures(bool flag = true) {textured = flag;}
    void useColors(bool flag = true) {colored = flag;}
    bool isTextured() { return textured; }
    bool isColored() { return colored; }

    virtual void bufferData(GLenum mode = GL_STATIC_DRAW);
    virtual void bufferData(bool flat, GLenum mode);
    void render(GLenum mode = GL_TRIANGLES);

    VertexArray vertexArray;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> faceNormals;
    std::vector<glm::vec3> normals;
    std::vector<TexCoord> texCoords;
    std::vector<Color> colors;
    std::vector<Face> faces;

    bool colored = false;
    bool textured = false;
    bool flatShading = false;

private:
    void initVertexArray();
};

}

#endif
