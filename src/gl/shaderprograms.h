/**
 * @file shaderprograms.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-12
 */

#ifndef SHADERPROGRAMS_H
#define SHADERPROGRAMS_H

#include "shaderprogram.h"

namespace rgl {

namespace ShaderPrograms {

rgl::ShaderProgram* createShader(const std::string& shaderName,
                                 const std::vector<rgl::VertexAttrib>& attribs);
rgl::ShaderProgram* createShader(const std::string& vertName,
                                 const std::string& fragName,
                                 const std::vector<rgl::VertexAttrib>& attribs);
bool initPrograms();

extern ShaderProgram* lightSphere;
extern ShaderProgram* mesh;
extern ShaderProgram* cubeMap;
extern ShaderProgram* reflect;
extern ShaderProgram* reflectL;

} // namespace
} // namespace

#endif
