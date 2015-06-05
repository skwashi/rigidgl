/**
 * @file assetmanager.cc
 *
 * @author Jonas Ransjö
 * @date 2015-05-30
 */

#include "assetmanager.h"

namespace rgl {

AssetManager assetManager;

AssetManager::~AssetManager()
{
    for (auto& it : textures)
        if (auto* ptr = it.second)
            delete ptr;

    for (auto& it : programs)
        if (auto* ptr = it.second)
            delete ptr;

    for (auto& it : meshes)
        if (auto* ptr = it.second)
            delete ptr;
}


void AssetManager::bindTexture(rgl::GLTexture *texture)
{
    if (texture != activeTexture) {
        if (texture)
            texture->bind();
        activeTexture = texture;
    }
}

void AssetManager::bindTexture(const std::string &textureName)
{
    bindTexture(textures[textureName]);
}

void AssetManager::useProgram(rgl::ShaderProgram *program)
{
    if (program != activeProgram) {
        if (program)
            program->use();
        activeProgram = program;
    }
}

} // namespace
