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
#include <cmath>

#include "math/rmath.h"
#include "../gl/vertex.h"
#include "../gl/vertexbuffer.h"

using namespace glm;

TerrainChunk::TerrainChunk(glm::vec3 offset, size_t size)
    : offset(offset), size(size)
{
    heights = new float*[size];
    normals = new glm::vec3*[size];
    texCoords = new glm::vec2*[size];

    for (size_t i = 0; i < size; i++) {
        heights[i] = new float[size];
        normals[i] = new glm::vec3[size];
        texCoords[i] = new glm::vec2[size];
        for (size_t j = 0; j < size; j++) {
            heights[i][j] = 0;
            normals[i][j] = {0, 0, 0};
            texCoords[i][j] = {0, 0};
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

float TerrainChunk::getHeight(float x, float z)
{
    float xmin, zmin;
    float s = std::modf(x, &xmin);
    float t = std::modf(z, &zmin);
    int i = xmin;
    int j = zmin;
    if (i < 0 || i > size - 2 ||
        j < 0 || j > size - 2)
        return -1;

    if (s >= t) {
        float A = heights[i][j];
        float B = heights[i][j+1];
        float C = heights[i+1][j+1];

        return A * (1 - s) + B * (s - t) + C * t;
    }

    else { // (t > s)
        float A = heights[i][j];
        float C = heights[i+1][j+1];
        float D = heights[i+1][j];

        return A * (1 - t) + D * (t - s) + C * s;
    }
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

void TerrainChunk::displace(noise::module::Module& noise)
{
    float val;

    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++) {
            val = noise.GetValue(offset.x + i,
                                 offset.y,
                                 offset.z + j);
            heights[i][j] += val;
        }
}

void TerrainChunk::displace(noise::module::Module& noise, float floor, float high)
{
    float val;
    float low = -1 + floor;

    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++) {
            val = floor + noise.GetValue(offset.x + i,
                                          offset.y,
                                          offset.z + j);
            heights[i][j] += max(0.0f, low + (high - low) * (val + (1 - floor)) / 2);
        }
}

void TerrainChunk::displace(noise::module::Module& noise, float ymin, float ymax, float low, float high)
{
    float val;

    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++) {
            if (heights[i][j] >= ymin && heights[i][j] <= ymax) {
                val = noise.GetValue(offset.x + i, offset.y, offset.z + j);
                heights[i][j] += low + (high - low) * (val + 1) / 2;
            }
        }
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
    vec3 p, v1, v2, v3, n;

    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - 1; j++) {
            p = vec3(i, heights[i][j], j);
            v1 = vec3(i, heights[i][j+1], j+1) - p;
            v2 = vec3(i+1, heights[i+1][j+1], j+1) - p;
            v3 = vec3(i+1, heights[i+1][j], j) - p;
            n = glm::normalize(cross(v1, v2));
            normals[i][j] += n;
            normals[i][j+1] += n;
            normals[i+1][j+1] += n;
            n = glm::normalize(cross(v2, v3));
            normals[i][j] += n;
            normals[i+1][j+1] += n;
            normals[i+1][j] += n;
        }
    }

    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++) {
            normals[i][j] = glm::normalize(normals[i][j]);
        }
}


void TerrainChunk::computeTexCoords()
{
    for (size_t i = 0; i < size - 1; i ++) {
        for (size_t j = 0; j < size - 1; j ++) {
            texCoords[i][j] = vec2(heights[i][j], 0);
        }
    }

}

void TerrainChunk::bufferData()
{
    float ox = offset.x;
    float oy = offset.y;
    float oz = offset.z;

    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - 1; j++) {
            vertexBuffer.addTriangle(
                {vec3(i + ox, heights[i][j] + oy, j + oz), normals[i][j], texCoords[i][j]},
                {vec3(i + ox, heights[i][j+1] + oy, j+1 + oz), normals[i][j+1], texCoords[i][j+1]},
                {vec3(i+1 + ox, heights[i+1][j+1] + oy, j+1 + oz), normals[i+1][j+1], texCoords[i+1][j+1]});
            vertexBuffer.addTriangle(
                {vec3(i + ox, heights[i][j] + oy, j + oz), normals[i][j], texCoords[i][j]},
                {vec3(i+1 + ox, heights[i+1][j+1] + oy, j+1 + oz), normals[i+1][j+1], texCoords[i+1][j+1]},
                {vec3(i+1 + ox, heights[i+1][j] + oy, j + oz), normals[i+1][j], texCoords[i+1][j]});
        }
    }
}


Terrain::Terrain(size_t chunkSize)
{
    using namespace noise::module;
    this->chunkSize = chunkSize;

    Billow baseFlatTerrain;
    baseFlatTerrain.SetFrequency(2.0 * 7 / chunkSize);
    ScaleBias flatTerrain;
    flatTerrain.SetSourceModule(0, baseFlatTerrain);
    flatTerrain.SetScale(0.125);
    flatTerrain.SetBias(-0.7);
    Perlin terrainType;
    terrainType.SetFrequency(0.5 * 7 / chunkSize);
    terrainType.SetPersistence(0.25 / chunkSize);
    RidgedMulti mountainTerrain;
    mountainTerrain.SetFrequency(0.5 * 7 / chunkSize);
    ScaleBias mTerrain;
    mTerrain.SetSourceModule(0, mountainTerrain);
    mTerrain.SetBias(0.3);
    Select& finalTerrain = noise;;
    finalTerrain.SetSourceModule(0, flatTerrain);
    finalTerrain.SetSourceModule(1, mTerrain);
    finalTerrain.SetControlModule(terrainType);
    finalTerrain.SetBounds(0, 1000);
    finalTerrain.SetEdgeFalloff(0.5);

    // Perlin perlin;
    // perlin.SetOctaveCount (6);
    // perlin.SetFrequency (0.5 * 2 * 7 / chunkSize);
    // perlin.SetPersistence (0.5);
    // chunk->displacePerlin(perlin, 0.6f, 1.6f);
    // perlin.SetOctaveCount (6);
    // perlin.SetFrequency (0.5 * 2 * 7 / chunkSize);
    // perlin.SetPersistence (0.5);
    //chunk->displacePerlin(perlin, 0.2f, 1.f, -0.2f, 0.5f);
    //chunk->normalize();

    TerrainChunk* chunk = new TerrainChunk(vec3(), chunkSize);
    chunk->displace(noise, 0.75f, 2.f);
    chunk->computeNormals();
    chunk->computeTexCoords();
    chunk->bufferData();
    chunks.push_back(chunk);
}

Terrain::~Terrain()
{
    for (TerrainChunk* chunk : chunks)
        delete chunk;
}

float Terrain::getHeight(vec3 pos)
{
    vec4 localPos = inverseModelMatrix * vec4(pos, 1);
    float x = localPos.x;
    float z = localPos.z;
    std::cout << x << " - " << z << std::endl;
    return getTop(pos).y;
}

vec3 Terrain::getTop(vec3 pos)
{
    vec4 localPos = inverseModelMatrix * vec4(pos, 1);
    float x = localPos.x, z = localPos.z;
    float y = chunks[0]->getHeight(x, z);
    return vec3(modelMatrix * vec4(x, y, z, 1));
}
