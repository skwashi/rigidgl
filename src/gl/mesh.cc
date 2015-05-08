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

using namespace rgl;
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

void Mesh::bufferData(GLenum usage)
{
    if (!vertexBuffer.isInited()) {
        initVertexBuffer();
    } else
        vertexBuffer.clear();

    if (normals.size() == 0)
        computeVertexNormals();

    assert(vertices.size() == normals.size());

    for (int i = 0, count = vertices.size(); i < count; i++) {
        float* vptr = value_ptr(vertices[i]);
        vertexBuffer.pushVertices(vptr, 3);
        float* nptr = value_ptr(normals[i]);
        vertexBuffer.pushVertices(nptr, 3);
        if (usingTextures()) {
            float* tptr = value_ptr(texCoords[i]);
            vertexBuffer.pushVertices(tptr, 2);
        }
        if (usingColors()) {
            float* cptr = value_ptr(colors[i]);
            vertexBuffer.pushVertices(cptr, 4);
        }
    }

    for (Face& face : faces) {
        int count = face.count;
        int start = face.vs[0];
        for (int i = 1; i < count - 1; i++) {
            vertexBuffer.addTriangleI(start, face.vs[i], face.vs[i+1]);
        }
    }

    vertexBuffer.bind();
    vertexBuffer.bufferData(usage);

    flatShading = false;
}

void Mesh::bufferData(bool flat, GLenum usage)
{
    if (flat == false) {
        bufferData(usage);
        return;
    }

    if (faceNormals.size() == 0)
        computeFaceNormals();

    if (!vertexBuffer.isInited())
        initVertexBuffer();
    else
        vertexBuffer.clear();

    int vertexCount = 0;

    for (int i = 0, faceCount = faces.size(); i < faceCount; i++) {
        Face face = faces[i];
        float* nptr = value_ptr(faceNormals[i]);
        for (int j = 0; j < face.count; j++) {
            int idx = face.vs[j];
            float* vptr = value_ptr(vertices[idx]);
            vertexBuffer.pushVertices(vptr, 3);
            vertexBuffer.pushVertices(nptr, 3);
            if (usingTextures()) {
                float* tptr = value_ptr(texCoords[idx]);
                vertexBuffer.pushVertices(tptr, 2);
            }
            if (usingColors()) {
                float* cptr = value_ptr(colors[idx]);
                vertexBuffer.pushVertices(cptr, 4);
            }
        }
        for (int j = 1; j < face.count - 1; j++) {
            vertexBuffer.addTriangleI(vertexCount, vertexCount + j, vertexCount + j + 1);
        }
        vertexCount += face.count;
    }

    vertexBuffer.bind();
    vertexBuffer.bufferData(usage);

    flatShading = true;
}

void Mesh::bufferFaceData(GLenum usage)
{
    bufferFaceData(false, usage);
}

void Mesh::bufferFaceData(bool flat, GLenum usage)
{
    if (!vertexBuffer.isInited()) {
        initVertexBuffer();
    } else
        vertexBuffer.clear();

    assert(vertices.size() == normals.size());

    int vertexCount = 0;

    for (int i = 0, fcount = faces.size(); i < fcount; i++) {
        Face face = faces[i];

        for (int j = 0; j < face.count; j++) {
            int v_j = face[j];

            float* vptr = value_ptr(vertices[v_j]);
            float* nptr = flat ?
                value_ptr(faceNormals[i]) :
                value_ptr(normals[v_j]);

            vertexBuffer.pushVertices(vptr, 3);
            vertexBuffer.pushVertices(nptr, 3);
            if (usingTextures()) {
                int t_j = face.ts ? face.ts[j] : v_j;
                float* tptr = value_ptr(texCoords[t_j]);
                vertexBuffer.pushVertices(tptr, 2);
            }

            if (usingColors()) {
                int c_j = face.cs ? face.cs[j] : v_j;
                float* cptr = value_ptr(colors[c_j]);
                vertexBuffer.pushVertices(cptr, 4);
            }

            for (int j = 1; j < face.count - 1; j++) {
                vertexBuffer.addTriangleI(vertexCount, vertexCount + j, vertexCount + j + 1);
            }
            vertexCount += face.count;
        }
    }

    vertexBuffer.bind();
    vertexBuffer.bufferData(usage);

    flatShading = flat;
}


void Mesh::render(GLenum mode)
{
    vertexBuffer.render();
}

void Mesh::initVertexBuffer() {
    if (usingTextures())
        if (usingColors())
            vertexBuffer.init(VAS_POSNORMTEXCOL);
        else
            vertexBuffer.init(VAS_POSNORMTEX);
    else if (usingColors())
        vertexBuffer.init(VAS_POSNORMCOL);
    else
        vertexBuffer.init(VAS_POSNORM);
}


