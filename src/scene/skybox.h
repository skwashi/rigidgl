/**
 * @file skybox.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-07
 */

#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>

#include "assets.h"
#include "meshes.h"
#include "../rutils.h"
#include "../gl/mesh.h"
#include "../gl/vertexattrib.h"
#include "../gl/cubemap.h"
#include "../gl/shaderprogram.h"

class Skybox
{
    const std::string VERTEX_SHADER = assets::SHADER_DIR + "skybox.vert";
    const std::string FRAGMENT_SHADER = assets::SHADER_DIR + "skybox.frag";

public:
    Skybox(rgl::CubeMap* cubemap);
    ~Skybox() {}

    rgl::ShaderProgram& getProgram()
    {
        return program;
    }

    void render();

private:
    rgl::CubeMap* cubemap;
    rgl::Mesh mesh;
    rgl::ShaderProgram program;
};

inline Skybox::Skybox(rgl::CubeMap* cubemap)
{
    this->cubemap = cubemap;
    rgl::createCubeMesh(&mesh);
    mesh.bufferData();
    program.create(rutils::readFile(VERTEX_SHADER),
                   rutils::readFile(FRAGMENT_SHADER),
                   rgl::VAS_POSNORM);
}

inline void Skybox::render()
{
    program.use();
    cubemap->bind();
    mesh.render();
}

#endif
