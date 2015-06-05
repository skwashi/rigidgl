/**
 * @file assetmanager.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-30
 */

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>

#include "gl/rgl.h"
#include "scene/rscene.h"

namespace rgl {

class AssetManager;

extern AssetManager assetManager;

class AssetManager
{
public:
    ~AssetManager();

    rgl::GLTexture* texture(const std::string& name) { return textures[name]; }
    rgl::ShaderProgram* program(const std::string& name) { return programs[name]; }
    rgl::Mesh* mesh(const std::string& name) { return meshes[name]; }

    void bindTexture(rgl::GLTexture* texture);
    void bindTexture(const std::string& textureName);
    void useProgram(rgl::ShaderProgram* program);
    void useProgram(const std::string& programName);

    std::map<std::string, rgl::GLTexture*> textures;
    std::map<std::string, rgl::ShaderProgram*> programs;
    std::map<std::string, rgl::Mesh*> meshes;

    rgl::GLTexture* activeTexture;
    rgl::ShaderProgram* activeProgram;
};

} //namespace
#endif
