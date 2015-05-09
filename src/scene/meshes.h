/**
 * @file meshes.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */

#ifndef MESHES_H
#define MESHES_H

#include <vector>
#include <glm/glm.hpp>

#include "../gl/mesh.h"

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

inline rgl::Mesh* createCubeMesh(rgl::Mesh* mesh = NULL)
{
    if (mesh == NULL)
        mesh = new rgl::MeshN();
    mesh->addVertices(&cubeV[0], cubeV.size());
    mesh->addFaces(&cubeF[0], cubeF.size());
    mesh->computeFaceNormals();
    mesh->computeVertexNormals();
    return mesh;
}

} //namespace

#endif
