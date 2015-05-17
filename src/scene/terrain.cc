/**
 * @file terrain.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-15
 */

#include "terrain.h"

#include <glm/glm.hpp>
#include <stdlib.h>
#include <time.h>
#include <noise/noise.h>

#include "math/rmath.h"
#include "../gl/vertex.h"
#include "../gl/vertexbuffer.h"

using namespace glm;

TerrainChunk::TerrainChunk(glm::vec3 offset, size_t size)
    : offset(offset), size(size)
{
    heights = new float*[size];
    normals = new glm::vec3*[size];

    for (size_t i = 0; i < size; i++) {
        heights[i] = new float[size];
        normals[i] = new glm::vec3[size];
        for (size_t j = 0; j < size; j++) {
            heights[i][j] = 0;
            normals[i][j] = {0, 0, 0};
        }
    }
};

TerrainChunk::~TerrainChunk()
{
    for (size_t i = 0; i < size; i++) {
        delete[] heights[i];
        delete[] normals[i];
    }

    delete[] heights;
    delete[] normals;
}

void TerrainChunk::zero()
{
    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++) {
            heights[i][j] = 0;
            normals[i][j] = {0, 0, 0};
        }
}

void TerrainChunk::displaceCircle(int x, int y, float radius)
{
    int min_x = rm::clamp<int>(x - radius, 0, size - 1);
    int max_x = rm::clamp<int>(x + radius, 0, size - 1);

    float dx, dy, d2, r2;
    r2 = radius * radius;

    for (int i = min_x; i <= max_x; i++)
        for (int j = min_x; j <= max_x; j++) {
            dx = i - x;
            dy = j - y;
            d2 = dx * dx + dy * dy;
            if (d2 <= r2)
                heights[i][j] +=  sqrt(r2 - d2);
        }
}

void TerrainChunk::displaceCircles(float min_r, float max_r, uint iterations)
{
    srand(time(NULL));
    size_t x, y;
    float radius;

    for (uint i = 0; i < iterations; i++) {
        x = rand() % size;
        y = rand() % size;
        radius = min_r + rand() / (float) RAND_MAX * (max_r - min_r);
        displaceCircle(x, y, radius);
    }
}

void TerrainChunk::displacePerlin(noise::module::Perlin& perlin)
{
    float scale = 7 / (float) size;
    double cap = 0.5;
    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++)
            heights[i][j] += cap +
                max(-cap,
                    perlin.GetValue(scale * (offset.x + i),
                                    scale * offset.y,
                                    scale * (offset.z + j)));

}

void TerrainChunk::normalize()
{
    float min, max, h;
    min = max = heights[0][0];
    for (size_t i = 0; i < size - 1; i++)
        for (size_t j = 0; j < size - 1; j++) {
            h = heights[i][j];
            if (h < min)
                min = h;
            if (h > max)
                max = h;
        }

    if (min == max)
        return;

    for (size_t i = 0; i < size - 1; i++)
        for (size_t j = 0; j < size - 1; j++) {
            h = heights[i][j];
            heights[i][j] = (h - min) / (max - min);
        }
}

void TerrainChunk::computeNormals()
{
    vec3 p, v1, v2, n;

    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - 1; j++) {
            p = vec3(i, heights[i][j], j);
            v1 = vec3(i, heights[i][j+1], j+1) - p;
            v2 = vec3(i+1, heights[i+1][j], j) - p;
            n = glm::normalize(cross(v1, v2));
            normals[i][j] += n;
            normals[i][j+1] += n;
            normals[i+1][j+1] += n;
            normals[i+1][j] += n;
        }
    }

    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++) {
            normals[i][j] = glm::normalize(normals[i][j]);
        }
}

void TerrainChunk::bufferData()
{
    float ox = offset.x;
    float oy = offset.y;
    float oz = offset.z;

    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - 1; j++) {
            vertexBuffer.addQuad(
                {vec3(i + ox, heights[i][j] + oy, j + oz), normals[i][j]},
                {vec3(i + ox, heights[i][j+1] + oy, j+1 + oz), normals[i][j+1]},
                {vec3(i+1 + ox, heights[i+1][j+1] + oy, j+1 + oz), normals[i+1][j+1]},
                {vec3(i+1 + ox, heights[i+1][j] + oy, j + oz), normals[i+1][j]});
        }
    }
}


Terrain::Terrain(size_t chunkSize)
{
    this->chunkSize = chunkSize;
    TerrainChunk* chunk = new TerrainChunk(vec3(), chunkSize);
    noise::module::Perlin perlin;
    chunk->displacePerlin(perlin);
    //chunk->normalize();
    chunk->computeNormals();
    chunk->bufferData();
    chunks.push_back(chunk);
}

Terrain::~Terrain()
{
    for (TerrainChunk* chunk : chunks)
        delete chunk;
}
