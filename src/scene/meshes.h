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
#include "../gl/vertexbuffer.h"

namespace rgl {

extern const std::vector<glm::vec3> cubeV;
extern const std::vector<rgl::Face> cubeF;

extern const std::vector<glm::vec3> quadV;

extern rgl::VBuffer<glm::vec3>* quadBuffer;
extern rgl::VBuffer<rgl::Vertex3t>* quadBufferT;

rgl::VBuffer<glm::vec3>* getQuadBuffer();

rgl::Mesh* createCubeMesh(rgl::Mesh* mesh = NULL);

} //namespace

#endif
