/**
 * @file shaderprograms.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-12
 */

#ifndef SHADERPROGRAMS_H
#define SHADERPROGRAMS_H

#include <vector>

#include "shaderprogram.h"

namespace rgl {

struct ShaderDef
{
    std::vector<std::string> vertFiles;
    std::vector<std::string> fragFiles;
    std::vector<VertexAttrib>& attribs;
};

namespace ShaderPrograms {

rgl::ShaderProgram* createShader(const ShaderDef& shaderDef);

rgl::ShaderProgram* createShader(const std::string& shaderName,
                                 const std::vector<rgl::VertexAttrib>& attribs);

rgl::ShaderProgram* createShader(const std::string& vertName,
                                 const std::string& fragName,
                                 const std::vector<rgl::VertexAttrib>& attribs);

rgl::ShaderProgram* createShader(const std::string* vertFiles, uint vfcount,
                                 const std::string* fragFiles, uint ffcount,
                                 const std::vector<rgl::VertexAttrib>& attribs);

rgl::ShaderProgram* createShader(const std::vector<std::string>& vertFiles,
                                 const std::vector<std::string>& fragFiles,
                                 const std::vector<rgl::VertexAttrib>& attribs);

bool initPrograms();

extern const std::string version130;

extern ShaderProgram* lightSphere;
extern ShaderProgram* mesh;
extern ShaderProgram* cubeMap;
extern ShaderProgram* reflect;
extern ShaderProgram* reflectL;
extern ShaderProgram* terrain;
extern ShaderProgram* terrainL;
extern ShaderProgram* depth;
extern ShaderProgram* depthUI;
extern ShaderProgram* normalUI;
extern ShaderProgram* positionUI;
extern ShaderProgram* UI;
extern ShaderProgram* gpmaterial;
extern ShaderProgram* gpterrain;
extern ShaderProgram* error;
extern ShaderProgram* error2;
extern ShaderProgram* lightPass;

} // namespace
} // namespace

#endif
