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

class Gamma : public GLApp
{
public:
    void init(int width, int height, const char* title = "Terrain Test");

protected:
    void prerender();
    void render();
    void destroy();
    void resizeCallback(GLFWwindow* window, int width, int height);
    void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    void initGBuffer();
    void initTerrain();
    void initLights();
    void initTweakBar();

    void renderGeometry();
    void renderLighting();
    void renderDisplay();

    struct {
        uint maxLights = 1024;
        uint numLights = 256;
        bool directionalLight = true;
        bool pointLights = true;
        bool renderLights = false;
        bool renderSkybox = true;
        bool specularLighting = true;
        bool diffuseLighting = true;
        bool showDepth = false;
        bool showNormals = false;
        bool showAlbedo = false;
        bool wireframe = false;
        glm::vec3 lightAttenuation;
        float lightRadius;
        glm::vec3 lightDirection;
        float heightAboveTerrain = 15;
        bool gamma = true;
    } settings;

    Light directionalLight;
    Terrain* terrain;
    Material material;
    Model model;
    Skybox* skybox;
};

#endif
