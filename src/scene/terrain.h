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
#include <utility>
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

    float getHeight(float x, float z);
    void zero();
    void displaceCircle(int x, int y, float radius);
    void displaceCircles(float min_r, float max_r, uint iterations);
    void displace(noise::module::Module& noise);
    void displace(noise::module::Module& noise, float floor, float high);
    void displace(noise::module::Module& noise, float hmin, float hmax, float low, float high);
    void normalize();
    void computeNormals();
    void computeTexCoords();
    void bufferData();

private:
    glm::vec3 offset;
    size_t size;
    float** heights;
    glm::vec3** normals;
    glm::vec2** texCoords;

    rgl::VBuffer<rgl::Vertex3nt> vertexBuffer;
};


class Terrain : public rgl::Renderable
{
public:
    Terrain(size_t chunkSize);
    ~Terrain();

    float getHeight(glm::vec3 pos);
    glm::vec3 getTop(glm::vec3 pos);

    void forAll(std::function<void(TerrainChunk*)> f)
    {
        for (TerrainChunk* chunk : chunks)
            f(chunk);
    }

    using rgl::Renderable::updateMatrices;
    void updateMatrices() {
        inverseModelMatrix = glm::inverse(modelMatrix);
        normalMatrix = glm::mat3(glm::transpose(inverseModelMatrix));
    }

    void render(rgl::ShaderProgram& program) {
        if (texture)
            texture->bind();
        updateUniforms(program);
        for (TerrainChunk* chunk : chunks)
            chunk->vertexBuffer.render();
    }
    using rgl::Renderable::render;

    size_t chunkSize;
    std::vector<TerrainChunk*> chunks;
    noise::module::Select noise;
    rgl::GLTexture* texture;
    glm::mat4 inverseModelMatrix;
};



#endif
