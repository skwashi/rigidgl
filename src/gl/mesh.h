/**
 * @author Jonas Ransjö
 */

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "rtypes.h"
#include "rgl.h"
#include "vertexattrib.h"
#include "vertexbuffer.h"

namespace rgl {

struct Face
{
    int count;
    int* vs;
    int* ts;
    int* cs;

    Face(int v1, int v2, int v3)
    {
        count = 3;
        vs = new int[3] {v1, v2, v3};
        ts = NULL;
        cs = NULL;
    }

    Face(int v1, int v2, int v3,
         int t1, int t2, int t3) : Face(v1, v2, v3)
    {
        ts = new int[3] {t1, t2, t3};
        cs = NULL;
    }

    Face(int v1, int v2, int v3,
         int t1, int t2, int t3,
         int c1, int c2, int c3) : Face(v1, v2, v3,
                                        t1, t2, t3)
    {
        cs = new int[3] {c1, c2, c3};
    }

    Face(int v1, int v2, int v3, int v4)
    {
        count = 4;
        vs = new int[4] {v1, v2, v3, v4};
        ts = NULL;
        cs = NULL;
    }

    Face(int v1, int v2, int v3, int v4,
         int t1, int t2, int t3, int t4) : Face(v1, v2, v3, v4)
    {
        ts = new int[4] {t1, t2, t3, t4};
        cs = NULL;
    }

    Face(int v1, int v2, int v3, int v4,
         int t1, int t2, int t3, int t4,
         int c1, int c2, int c3, int c4) : Face(v1, v2, v3, v4,
                                                t1, t2, t3, t4)
    {
        cs = new int[4] {c1, c2, c3, c4};
    }

    Face(int _count, const int* _vs)
    {
        count = _count;
        vs = new int[count];
        for (int i = 0; i < count; i++)
            vs[i] = _vs[i];
        ts = NULL;
        cs = NULL;
    }

    Face(int _count, const int* _vs, const int* _ts)
    {
        count = _count;
        vs = new int[count];
        ts = new int[count];
        for (int i = 0; i < count; i++) {
            vs[i] = _vs[i];
            ts[i] = _ts[i];
        }
        cs = NULL;
    }

    Face(const Face& face) {
        count = face.count;
        vs = new int[count];
        for (int i = 0; i < count; i++)
            vs[i] = face.vs[i];
        if (face.ts != NULL) {
            ts = new int[count];
            for (int i = 0; i < count; i++)
                ts[i] = face.ts[i];
        } else
            ts = NULL;
        if (face.cs != NULL) {
            cs = new int[count];
            for (int i = 0; i < count; i++)
                cs[i] = face.cs[i];
        } else
            cs = NULL;
    }

    ~Face()
    {
        delete[] vs;
        if (ts != NULL)
            delete[] ts;
        if (cs != NULL)
            delete[] cs;
    }

    int operator[](int i)
    {
        return vs[i];
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
    void addTexCoord(const TexCoord& t);
    void addTexCoords(const TexCoord* ts, int count);
    void addColor(const Color& c);
    void addColors(const Color* cs, int count);
    void addFace(const Face& face);
    void addFaces(const Face* faces, int count);
    void clearVertices();
    void clearNormals();
    void clearTexCoords();
    void clearColors();
    void clearFaces();
    void clear();
    void computeFaceNormals();
    void computeVertexNormals();

    void useTextures(bool flag = true) {textured = flag;}
    void useColors(bool flag = true) {colored = flag;}
    bool usingTextures() { return textured; }
    bool usingColors() { return colored; }

    virtual void bufferData(GLenum usage = GL_STATIC_DRAW);
    virtual void bufferData(bool flat, GLenum usage);
    virtual void bufferFaceData(GLenum usage = GL_STATIC_DRAW);
    virtual void bufferFaceData(bool flat, GLenum usage);
    void render(GLenum mode = GL_TRIANGLES);

    VertexBuffer vertexBuffer;
    std::vector<glm::vec3> vertices;
    std::vector<TexCoord> texCoords;
    std::vector<Color> colors;
    std::vector<glm::vec3> faceNormals;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;

    bool colored = false;
    bool textured = false;
    bool flatShading = false;

private:
    void initVertexBuffer();
};

}

#endif
