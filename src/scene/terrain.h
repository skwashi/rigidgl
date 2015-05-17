/**
 * @file terrain.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-15
 */

#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <noise/noise.h>
#include <vector>
#include <functional>

#include "math/glmutils.h"
#include "../gl/vertex.h"
#include "../gl/vertexbuffer.h"
#include "../gl//renderable.h"
#include "../gl/shaderprogram.h"
#include "../gl/gltexture.h"

class Terrain;

class TerrainChunk
{
public:
    friend class Terrain;

    TerrainChunk(glm::vec3 offset, size_t size);
    ~TerrainChunk();

    void zero();
    void displaceCircle(int x, int y, float radius);
    void displaceCircles(float min_r, float max_r, uint iterations);
    void displacePerlin(noise::module::Perlin& perlin);
    void normalize();
    void computeNormals();
    void bufferData();

private:
    glm::vec3 offset;
    size_t size;
    float** heights;
    glm::vec3** normals;

    rgl::VBuffer<rgl::Vertex3n> vertexBuffer;
};


class Terrain : public rgl::Renderable
{
public:
    Terrain(size_t chunkSize);
    ~Terrain();

    void forAll(std::function<void(TerrainChunk*)> f)
    {
        for (TerrainChunk* chunk : chunks)
            f(chunk);
    }

    void updateMatrices() {
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
    }

    void render(rgl::ShaderProgram& program) {
        if (texture)
            texture->bind();
        updateUniforms(program);
        for (TerrainChunk* chunk : chunks)
            chunk->vertexBuffer.render();
    }

    size_t chunkSize;
    std::vector<TerrainChunk*> chunks;
    rgl::GLTexture* texture;
};



#endif
