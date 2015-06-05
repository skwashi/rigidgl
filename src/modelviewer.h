/**
 * @file modelviewer.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-12
 */

#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <AntTweakBar.h>
#include <glm/glm.hpp>

#include "glapp.h"
#include "gl/mesh.h"
#include "gl/shaderprogram.h"
#include "gl/cubemap.h"
#include "gl/framebuffer.h"
#include "scene/model.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/skybox.h"
#include "input/Trackball.h"
#include "math/rmath.h"

class ModelViewer : public GLApp
{
public:
    void init(int width, int height, const char* title = "Model Viewer");

protected:
    void prepare();
    void update();
    void handleInput();
    void updateCamera();
    void render();
    void destroy();
    void resizeCallback(GLFWwindow* window, int width, int height);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    void initTweakBar();
    void initLights();
    void initMeshes();
    void initCubeMaps();
    void initFrameBuffer();

    struct
    {
        float zoomFactor = 1;
        float fov = 45.f;
        glm::vec4 clearColor = glm::vec4(0, 0, 0, 1);
        glm::vec3 lightPosition;
        bool ambient = true;
        bool diffuse = true;
        bool specular = true;
        bool attenuation = true;
        bool gamma = true;
        bool invertColor = false;
        bool normalColors = false;
        bool perspective = true;
        bool tweakBarVisible = true;
        bool renderLights = true;
        bool renderSkybox = false;
        bool highres = false;
        int activeCMap = 0;
    } settings;

    struct CMap
    {
        rgl::CubeMap* highres;
        rgl::CubeMap* filtered0125;
        rgl::CubeMap* filtered05;
        rgl::CubeMap* filtered2;
        rgl::CubeMap* filtered8;
        rgl::CubeMap* filtered32;
        rgl::CubeMap* filtered128;
        rgl::CubeMap* filtered512;
        rgl::CubeMap* filtered2048;
    } cmaps[3];

    rgl::CubeMap* getCubeMap(float shininess);

    cgtk::Trackball trackball;
    rgl::MeshN* meshes[5];
    Light* light;
    Model* model;
    Material material;
    rgl::ShaderProgram* program;
    Skybox* skybox;
};

#endif
