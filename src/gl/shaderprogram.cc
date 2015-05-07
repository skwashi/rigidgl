/**
 * @author Jonas Ransjö
 * Based on GLSLprogram by Johan Nysjö
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertexattrib.h"
#include "vertexarray.h"
#include "shaderprogram.h"

namespace {

    std::string  shaderType(GLenum type) {
        if (type == GL_VERTEX_SHADER)
            return "GL_VERTEX_SHADER";
        else if (type == GL_FRAGMENT_SHADER)
            return "GL_FRAGMENT_SHADER";
        else
            return "UNKNOWN SHADER";
    }

    void showShaderInfoLog(GLuint shader, GLenum type)
    {
        if (!glIsShader(shader)) {
            std::cerr << "Input argument is not a shader." << std::endl;
            return;
        }

        GLint infoLogLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            char* infoLog = (char*)malloc(infoLogLength * sizeof(char));
            glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
            std::cerr << "Error compiling " + shaderType(type) + ":\n" << infoLog << std::endl;
            free(infoLog);
        }
    }

    GLuint compileShader(const char* shaderSource, GLenum type)
    {
        GLuint shader = glCreateShader(type);
        if (!shader)
            return 0;

        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            showShaderInfoLog(shader, type);
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }


    void showProgramInfoLog(GLuint program)
    {
        if (!glIsProgram(program)) {
            std::cerr << "Input argument is not a program." << std::endl;
            return;
        }

        GLint infoLogLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            char* infoLog = (char*)malloc(infoLogLength * sizeof(char));
            glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
            std::cerr << "Error linking program:\n" << infoLog << std::endl;
            free(infoLog);
        }
    }

    bool linkProgram(GLuint program)
    {
        glLinkProgram(program);
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            showProgramInfoLog(program);
            glDeleteProgram(program);
            return false;
        }
        return true;
    }

    bool validateProgram(GLuint program)
    {
        glValidateProgram(program);
        GLint validated;
        glGetProgramiv(program, GL_VALIDATE_STATUS, &validated);
        if (!validated) {
            std::cerr << "Invalid program." << std::endl;
            glDeleteProgram(program);
            return false;
        }
        return true;
    }
}

using namespace rgl;

ShaderProgram::ShaderProgram()
{
    programId = 0;
}

ShaderProgram::~ShaderProgram()
{
    if (programId != 0)
        glDeleteProgram(programId);
}

bool ShaderProgram::create(const std::string& vertSource,
                           const std::string& fragSource)
{
    this->vertSource = vertSource;
    this->fragSource = fragSource;
    if (!compile())
        return false;
    if (!link())
        return false;
    cacheAttribs();
    cacheUniforms();
    return true;
}

bool ShaderProgram::create(const std::string& vertSource,
                           const std::string& fragSource,
                           const std::vector<VertexAttrib>& attribs)
{
    this->vertSource = vertSource;
    this->fragSource = fragSource;
    if (!compile())
        return false;
    for (VertexAttrib attrib : attribs) {
        glBindAttribLocation(programId, attrib.location, attrib.name.c_str());
    }
    if (!link())
        return false;
    cacheAttribs();
    cacheUniforms();
    return true;
}

bool ShaderProgram::create(const std::string& vertSource,
                            const std::string& fragSource,
                            const VertexAttrib* attribs,
                            int count)
{
    this->vertSource = vertSource;
    this->fragSource = fragSource;
    if (!compile())
        return false;
    for (int i = 0; i < count; i++) {
        VertexAttrib attrib = attribs[i];
        glBindAttribLocation(programId, attrib.location, attrib.name.c_str());
    }
    if (!link())
        return false;
    cacheAttribs();
    cacheUniforms();
    return true;
}

bool ShaderProgram::isValid() const
{
    return validateProgram(programId);
}

void ShaderProgram::use() const
{
    glUseProgram(programId);
}

void ShaderProgram::disable() const
{
    glUseProgram(0);
}

GLuint ShaderProgram::getId() const
{
    return programId;
}

bool ShaderProgram::hasAttrib(const std::string& name) const
{
    return attribs.count(name) > 0;
}

bool ShaderProgram::hasUniform(const std::string& name) const
{
    return uniforms.count(name) > 0;
}

GLint ShaderProgram::getAttribLocation(const std::string& name) const
{
    auto found = attribs.find(name);
    if (found == attribs.end())
        return -1;
    return found->second;
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const
{
    auto found = uniforms.find(name);
    if (found == uniforms.end())
        return -1;
    return found->second;
}

// uniform setters

void ShaderProgram::setUniform1i(int loc, int val)
{
    glUniform1i(loc, val);
}

void ShaderProgram::setUniform2i(int loc, const glm::ivec2& val)
{
    glUniform2iv(loc, 1, glm::value_ptr(val));
}

void ShaderProgram::setUniform3i(int loc, const glm::ivec3& val)
{
    glUniform3iv(loc, 1, glm::value_ptr(val));
}

void ShaderProgram::setUniform4i(int loc, const glm::ivec4& val)
{
    glUniform4iv(loc, 1, glm::value_ptr(val));
}

void ShaderProgram::setUniform1f(int loc, float val)
{
    glUniform1f(loc, val);
}

void ShaderProgram::setUniform2f(int loc, const glm::vec2& val)
{
    glUniform2fv(loc, 1, glm::value_ptr(val));
}

void ShaderProgram::setUniform3f(int loc, const glm::vec3& val)
{
    glUniform3fv(loc, 1, glm::value_ptr(val));
}

void ShaderProgram::setUniform4f(int loc, const glm::vec4& val)
{
    glUniform4fv(loc, 1, glm::value_ptr(val));
}

// uniform string setters

void ShaderProgram::setUniform1i(const std::string& name, int val)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniform1i(loc, val);
}

void ShaderProgram::setUniform2i(const std::string& name, const glm::ivec2& val)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniform2i(loc, val);
}

void ShaderProgram::setUniform3i(const std::string& name, const glm::ivec3& val)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniform3i(loc, val);
}

void ShaderProgram::setUniform4i(const std::string& name, const glm::ivec4& val)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniform4i(loc, val);
}

void ShaderProgram::setUniform1f(const std::string& name, float val)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniform1f(loc, val);
}

void ShaderProgram::setUniform2f(const std::string& name, const glm::vec2& val)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniform2f(loc, val);
}

void ShaderProgram::setUniform3f(const std::string& name, const glm::vec3& val)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniform3f(loc, val);
}

void ShaderProgram::setUniform4f(const std::string& name, const glm::vec4& val)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniform4f(loc, val);
}

// uniform matrix setters

void ShaderProgram::setUniformMatrix2f(int loc, const glm::mat2& mat,
                                       bool transpose)
{
    glUniformMatrix2fv(loc, 1, transpose, glm::value_ptr(mat));
}

void ShaderProgram::setUniformMatrix3f(int loc, const glm::mat3& mat,
                                       bool transpose)
{
    glUniformMatrix3fv(loc, 1, transpose, glm::value_ptr(mat));
}

void ShaderProgram::setUniformMatrix4f(int loc, const glm::mat4& mat,
                                       bool transpose)
{
    glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(mat));
}

void ShaderProgram::setUniformMatrix2f(const std::string& name, const glm::mat2& mat,
                                       bool transpose)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniformMatrix2f(loc, mat, transpose);
}

void ShaderProgram::setUniformMatrix3f(const std::string& name,
                        const glm::mat3& mat, bool transpose)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniformMatrix3f(loc, mat, transpose);
}

void ShaderProgram::setUniformMatrix4f(const std::string& name,
                        const glm::mat4& mat, bool transpose)
{
    int loc = getUniformLocation(name);
    if (loc != -1)
        setUniformMatrix4f(loc, mat, transpose);
}

bool ShaderProgram::compile()
{
    programId = glCreateProgram();
    if (!programId)
        return false;

    vertexShader = compileShader(vertSource.c_str(), GL_VERTEX_SHADER);
    if (!vertexShader)
        return false;

    fragmentShader = compileShader(fragSource.c_str(), GL_FRAGMENT_SHADER);
    if (!fragmentShader)
        return false;

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);

    return true;
}

bool ShaderProgram::link()
{
    if (!linkProgram(programId))
        return false;

    if (!validateProgram(programId))
        return false;

    glDetachShader(programId, vertexShader);
    glDetachShader(programId, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void ShaderProgram::cacheAttribs()
{
    attribs.clear();
    int count, maxLength;
    glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &count);
    glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    char name[maxLength + 1];
    for (int i = 0; i < count; i++) {
        int len, size;
        GLenum type;
        glGetActiveAttrib(programId, GLuint(i), maxLength, &len, &size, &type, name);
        name[len] = 0;
        int loc = glGetAttribLocation(programId, name);
        attribs[name] = loc;
    }
}

void ShaderProgram::cacheUniforms()
{
    uniforms.clear();
    int count, maxLength;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
    char name[maxLength + 1];
    for (int i = 0; i < count; i++) {
        int len, size;
        GLenum type;
        glGetActiveUniform(programId, GLuint(i), maxLength, &len, &size, &type, name);
        name[len] = 0;
        int loc = glGetUniformLocation(programId, name);
        uniforms[name] = loc;
    }
}
