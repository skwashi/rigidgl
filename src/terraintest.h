/**
 * @file terraintest.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-16
 */

#ifndef TERRAINTEST_H
#define TERRAINTEST_H

#include "glapp.h"
#include "scene/terrain.h"
#include "scene/skybox.h"

class TerrainTest : public GLApp
{
public:
    void init(int width, int height, const char* title = "Terrain Test");
    void render();

protected:
    Terrain* terrain;
    Skybox* skybox;
};

#endif
