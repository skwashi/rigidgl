/**
 * @author Jonas Ransjö
 */

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GL/glew.h>

#include "vertexattrib.h"
#include "vertexarray.h"
#include "mesh.h"
#include "vertex.h"

namespace rgl {

using namespace glm;

void Mesh::addVertex(const glm::vec3& v)
{
    vertices.push_back(v);
}

void Mesh::addVertices(const glm::vec3* vs, int count)
{
    for (int i = 0; i < count; i++)
        vertices.push_back(vs[i]);
}

void Mesh::addNormal(const glm::vec3& n)
{
    normals.push_back(n);
}

void Mesh::addNormals(const glm::vec3* ns, int count)
{
    for (int i = 0; i < count; i++)
        normals.push_back(ns[i]);
}

void Mesh::addTexCoord(const TexCoord& t)
{
    texCoords.push_back(t);
}

void Mesh::addTexCoords(const TexCoord* ts, int count)
{
    for (int i = 0; i < count; i++)
        texCoords.push_back(ts[i]);
}

void Mesh::addColor(const Color& c)
{
    colors.push_back(c);
}

void Mesh::addColors(const Color* cs, int count)
{
    for (int i = 0; i < count; i++)
        colors.push_back(cs[i]);
}

void Mesh::addFace(const Face& f)
{
    faces.push_back(f);
}

void Mesh::addFaces(const Face* fs, int count)
{
    for (int i = 0; i < count; i++)
        faces.push_back(fs[i]);
}

void Mesh::clearVertices()
{
    vertices.clear();
}

void Mesh::clearNormals()
{
    faceNormals.clear();
    normals.clear();
}

void Mesh::clearTexCoords()
{
    texCoords.clear();
}

void Mesh::clearColors()
{
    colors.clear();
}

void Mesh::clearFaces()
{
    faces.clear();
}

void Mesh::clear()
{
    clearVertices();
    clearNormals();
    clearTexCoords();
    clearColors();
    clearFaces();
}

void Mesh::computeFaceNormals()
{
    faceNormals.clear();
    faceNormals.reserve(faces.size());
    for (Face& face : faces) {
        vec3 v1 = vertices[face.vs[1]] - vertices[face.vs[0]];
        vec3 v2 = vertices[face.vs[2]] - vertices[face.vs[0]];
        vec3 n = normalize(cross(v1, v2));
        faceNormals.push_back(n);
    }
}

void Mesh::computeVertexNormals()
{
    if (faceNormals.size() == 0)
        computeFaceNormals();

    int vertexCount = vertices.size();
    normals.clear();
    normals.reserve(vertexCount);

    for (int i = 0; i < vertexCount; i++) {
        normals.push_back(vec3(0, 0, 0));
    }

    for (int i = 0, count = faces.size(); i < count; i++) {
        Face face = faces[i];
        vec3 n = faceNormals[i];

        for (int j = 0; j < face.count; j++) {
            int idx= face.vs[j];
            normals[idx] += n;
        }
    }

    for (int i = 0, count = normals.size(); i < count; i++)
        normals[i] = normalize(normals[i]);
}

void Mesh::render(GLenum mode)
{
    vertexBuffer->render();
}

// VMesh template instantiation

template <typename V>
void VMesh<V>::bufferData(GLenum usage)
{
    vbuffer->clear();

    if (normals.size() == 0)
        computeVertexNormals();

    for (int i = 0, count = vertices.size(); i < count; i++) {
        bufferVertex(i);
    }
    for (Face& face : faces) {
        int fcount = face.count;
        int start = face.vs[0];
        for (int i = 1; i < fcount - 1; i++)
            vbuffer->addTriangleI(start, face.vs[i], face.vs[i + 1]);
    }

    vbuffer->bind();
    vbuffer->bufferData(usage);
    flatShading = false;
}

template <typename V>
void VMesh<V>::bufferData(bool flat, GLenum usage)
{
    if (flat == false) {
        bufferData(usage);
        return;
    }

    vbuffer->clear();

    if (faceNormals.size() == 0)
        computeFaceNormals();

    int vertexCount = 0;

    for (int i = 0, faceCount = faces.size(); i < faceCount; i++) {
        Face face = faces[i];
        for (int j = 0; j < face.count; j++) {
            int vj = face.vs[j];
            bufferVertex(vj, i, vj, vj, true);
        }
        for (int j = 1; j < face.count - 1; j++) {
            vbuffer->addTriangleI(vertexCount, vertexCount + j, vertexCount + j + 1);
        }
        vertexCount += face.count;
    }

    vbuffer->bind();
    vbuffer->bufferData(usage);

    flatShading = true;
}

template <typename V>
void VMesh<V>::bufferFaceData(bool flat, GLenum usage)
{
    vbuffer->clear();

    if (flat && faceNormals.size() == 0)
        computeFaceNormals();
    if (!flat && normals.size() == 0)
        computeVertexNormals();

    int vertexCount = 0;

    for (int i = 0, fcount = faces.size(); i < fcount; i++) {
        Face face = faces[i];
        for (int j = 0; j < face.count; j++) {
            int vj = face[j];
            int nj = flat ? i : vj;
            int tj = face.ts ? face.ts[j] : vj;
            int cj = face.cs ? face.cs[j] : vj;
            bufferVertex(vj, nj, tj, cj, flat);
        }
        for (int j = 1; j < face.count - 1; j++) {
            vbuffer->addTriangleI(vertexCount, vertexCount + j, vertexCount + j + 1);
        }
        vertexCount += face.count;
    }

    vbuffer->bind();
    vbuffer->bufferData(usage);

    flatShading = flat;
}

template <>
inline void VMesh<Vertex3n>::bufferVertex(int vi, int ni, int ti, int ci,
    bool flat)
{
    vec3& n = flat ? faceNormals[ni] : normals[ni];
    vbuffer->pushVertex({vertices[vi], n});
}

template <>
inline void VMesh<Vertex3nt>::bufferVertex(int vi, int ni, int ti, int ci,
    bool flat)
{
    vec3& n = flat ? faceNormals[ni] : normals[ni];
    vbuffer->pushVertex({vertices[vi], n, texCoords[ti]});
}

template <>
inline void VMesh<Vertex3nc>::bufferVertex(int vi, int ni, int ti, int ci,
    bool flat)
{
    vec3& n = flat ? faceNormals[ni] : normals[ni];
    vbuffer->pushVertex({vertices[vi], n, colors[ci]});
}

template <>
inline void VMesh<Vertex3ntc>::bufferVertex(int vi, int ni, int ti, int ci,
    bool flat)
{
    vec3& n = flat ? faceNormals[ni] : normals[ni];
    vbuffer->pushVertex({vertices[vi], n, texCoords[ti], colors[ci]});
}

template class VMesh<Vertex3n>;
template class VMesh<Vertex3nt>;
template class VMesh<Vertex3nc>;
template class VMesh<Vertex3ntc>;

} // namespace
