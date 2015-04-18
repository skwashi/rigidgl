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

void Mesh::bufferData(GLenum mode)
{
    if (!vertexArray.isInited()) {
        initVertexArray();
    } else
        vertexArray.clear();

    assert(vertices.size() == normals.size());

    for (int i = 0, count = vertices.size(); i < count; i++) {
        float* vptr = value_ptr(vertices[i]);
        vertexArray.vertices.insert(vertexArray.vertices.end(),
                                    vptr, vptr + 3);
        float* nptr = value_ptr(normals[i]);
        vertexArray.vertices.insert(vertexArray.vertices.end(),
                                    nptr, nptr + 3);
        if (isTextured()) {
            float* tptr = value_ptr(texCoords[i]);
            vertexArray.vertices.insert(vertexArray.vertices.end(),
                                        tptr, tptr + 2);
        }
        if (isColored()) {
            float* cptr = value_ptr(colors[i]);
            vertexArray.vertices.insert(vertexArray.vertices.end(),
                                        cptr, cptr + 4);
        }
    }

    for (Face& face : faces) {
        int count = face.count;
        int start = face.vs[0];
        for (int i = 1; i < count - 1; i++) {
            vertexArray.indices.push_back(start);
            vertexArray.indices.push_back(face.vs[i]);
            vertexArray.indices.push_back(face.vs[i+1]);
        }
    }

    vertexArray.bind();
    vertexArray.bufferData(mode);
    vertexArray.unbind();

    flatShading = false;
}

void Mesh::bufferData(bool flat, GLenum mode)
{
    if (flat == false) {
        bufferData(mode);
        return;
    }

    if (faceNormals.size() == 0)
        computeFaceNormals();

    if (!vertexArray.isInited())
        initVertexArray();
    else
        vertexArray.clear();

    int vertexCount = 0;

    for (int i = 0, faceCount = faces.size(); i < faceCount; i++) {
        Face face = faces[i];
        float* nptr = value_ptr(faceNormals[i]);
        for (int j = 0; j < face.count; j++) {
            int idx = face.vs[j];
            float* vptr = value_ptr(vertices[idx]);
            vertexArray.vertices.insert(vertexArray.vertices.end(),
                                        vptr, vptr + 3);
            vertexArray.vertices.insert(vertexArray.vertices.end(),
                                        nptr, nptr + 3);
            if (isTextured()) {
                float* tptr = value_ptr(texCoords[idx]);
                vertexArray.vertices.insert(vertexArray.vertices.end(),
                                            tptr, tptr + 2);
            }
            if (isColored()) {
                float* cptr = value_ptr(colors[idx]);
                vertexArray.vertices.insert(vertexArray.vertices.end(),
                                            cptr, cptr + 4);
            }
        }
        for (int j = 1; j < face.count - 1; j++) {
            vertexArray.indices.push_back(vertexCount);
            vertexArray.indices.push_back(vertexCount + j);
            vertexArray.indices.push_back(vertexCount + j + 1);
        }
        vertexCount += face.count;
    }

    vertexArray.bind();
    vertexArray.bufferData(mode);
    vertexArray.unbind();

    flatShading = true;
}

void Mesh::render(GLenum mode)
{
    vertexArray.bind();
    vertexArray.drawElements(mode);
    vertexArray.unbind();
}

void Mesh::initVertexArray() {
    if (isTextured())
        if (isColored())
            vertexArray = VertexArray(VAS_POSNORMTEXCOL);
        else
            vertexArray = VertexArray(VAS_POSNORMTEX);
    else if (isColored())
        vertexArray = VertexArray(VAS_POSNORMCOL);
    else
        vertexArray = VertexArray(VAS_POSNORM);
}
