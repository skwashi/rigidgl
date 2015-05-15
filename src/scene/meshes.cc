/**
 * @file meshes.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-12
 */

#include <vector>
#include <glm/glm.hpp>

#include "meshes.h"
#include "../gl/mesh.h"
#include "../gl/vertexbuffer.h"

namespace rgl {

const std::vector<glm::vec3> cubeV = {
    glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1),
    glm::vec3(1, -1, 1), glm::vec3(1, 1, 1),
    glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1),
    glm::vec3(1, -1, -1), glm::vec3(1, 1, -1)
};

const std::vector<rgl::Face> cubeF = {
    rgl::Face(0, 1, 2, 3),
    rgl::Face(7, 6, 5, 4),
    rgl::Face(4, 5, 1, 0),
    rgl::Face(3, 2, 6, 7),
    rgl::Face(4, 0, 3, 7),
    rgl::Face(1, 5, 6, 2)
};

const std::vector<glm::vec3> quadV = {
    glm::vec3(-1, 1, 0),
    glm::vec3(-1, -1, 0),
    glm::vec3(1, -1, 0),
    glm::vec3(1, 1, 0)
};

rgl::VBuffer<glm::vec3>* quadBuffer;

rgl::VBuffer<glm::vec3>* getQuadBuffer()
{
    if (quadBuffer == NULL) {
            quadBuffer = new rgl::VBuffer<glm::vec3>(GL_STATIC_DRAW);
            quadBuffer->addQuad(quadV[0], quadV[1], quadV[2], quadV[3]);
    }
    return quadBuffer;
}

rgl::Mesh* createCubeMesh(rgl::Mesh* mesh)
{
    if (mesh == NULL)
        mesh = new rgl::MeshN();
    mesh->addVertices(&cubeV[0], cubeV.size());
    mesh->addFaces(&cubeF[0], cubeF.size());
    mesh->computeFaceNormals();
    mesh->computeVertexNormals();
    return mesh;
}

} // namespace
