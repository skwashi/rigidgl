/**
 * @author Jonas Ransjö
 */

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "vertexattrib.h"
#include "vertexarray.h"

namespace rgl {

class ShaderProgram
{
public:

    ShaderProgram();
    ~ShaderProgram();

    bool create(const std::string& vertSource,
                 const std::string& fragSource);

    bool create(const std::string& vertSource,
                 const std::string& fragSource,
                 const std::vector<VertexAttrib>& attribs);

    bool create(const std::string& vertSource,
                 const std::string& fragSource,
                 const VertexAttrib* attribs, int count);

    bool isValid() const; 
    void use() const;
    void disable() const;
    GLuint getId() const;
    bool hasAttrib(const std::string& name) const;
    bool hasUniform(const std::string& name) const;
    GLint getAttribLocation(const std::string& name) const;
    GLint getUniformLocation(const std::string& name) const;

    // uniform setters

    void setUniform1i(int loc, int val);
    void setUniform2i(int loc, const glm::ivec2& val);
    void setUniform3i(int loc, const glm::ivec3& val);
    void setUniform4i(int loc, const glm::ivec4& val);
    void setUniform1f(int loc, float val);
    void setUniform2f(int loc, const glm::vec2& val);
    void setUniform3f(int loc, const glm::vec3& val);
    void setUniform4f(int loc, const glm::vec4& val);

    // uniform string setters
    void setUniform1i(const std::string& name, int val);
    void setUniform2i(const std::string& name, const glm::ivec2& val);
    void setUniform3i(const std::string& name, const glm::ivec3& val);
    void setUniform4i(const std::string& name, const glm::ivec4& val);
    void setUniform1f(const std::string& name, float val);
    void setUniform2f(const std::string& name, const glm::vec2& val);
    void setUniform3f(const std::string& name, const glm::vec3& val);
    void setUniform4f(const std::string& name, const glm::vec4& val);

    // uniform matrix setters
    void setUniformMatrix2f(int loc, const glm::mat2& mat,
                            bool transpose = false);
    void setUniformMatrix3f(int loc, const glm::mat3& mat,
                            bool transpose = false);
    void setUniformMatrix4f(int loc, const glm::mat4& mat,
                            bool transpose = false);
    void setUniformMatrix2f(const std::string& name, const glm::mat2& mat,
                            bool transpose = false);
    void setUniformMatrix3f(const std::string& name, const glm::mat3& mat,
                            bool transpose = false);
    void setUniformMatrix4f(const std::string& name, const glm::mat4& mat,
                            bool transpose = false);

private:
    GLuint programId;
    std::string vertSource;
    std::string fragSource;
    GLuint vertexShader;
    GLuint fragmentShader;
    std::unordered_map<std::string, int> attribs;
    std::unordered_map<std::string, int> uniforms;

    bool compile();
    bool link();
    void cacheAttribs();
    void cacheUniforms();
};

}


#endif
