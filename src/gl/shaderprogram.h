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

extern const std::string U_PROJMATRIX; //= "u_p";
extern const std::string U_VIEWMATRIX; //= "u_v";
extern const std::string U_MODELMATRIX; //= "u_m";
extern const std::string U_PROJVIEWMATRIX; //= "u_pv";
extern const std::string U_MODELVIEWMATRIX; //= "u_mv";
extern const std::string U_MVPMATRIX; //= "u_mvp";
extern const std::string U_NORMALMATRIX; //= "u_normal";
extern const std::string U_TEXTURESAMPLER; //= "u_textureSampler";
extern const std::string U_FLAGS; // = "u_flags"

class ShaderProgram
{
public:

    enum SHADER_FLAGS
    {
        ALL = ~0,
        BASE = 1 << 0,
        AMBIENT = 1 << 2,
        DIFFUSE = 1 << 3,
        SPECULAR = 1 << 4,
        ATTENUATION = 1 << 5,
        GAMMA = 1 << 6,
        INVERTCOLOR = 1 << 7,
        NORMALCOLORS = 1 << 8,
        STANDARD = AMBIENT | DIFFUSE | SPECULAR | ATTENUATION | GAMMA,
        NONE = 0
    };

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

    int getFlags() { return flags; }

    void setFlags(int flags) {
        if (flags != this->flags) {
            this->flags = flags;
            flagsChanged = true;
        }
    }

    void addFlags(int flag)
    {
        int oldFlags = flags;
        flags |= flag;
        if (flags != oldFlags)
            flagsChanged = true;
    }

    void removeFlags(int flag)
    {
        int oldFlags = flags;
        flags &= !flag;
        if (flags != oldFlags)
            flagsChanged = true;
    }

    void setUniformFlags()
    {
        if (flagsChanged && hasUniform(U_FLAGS)) {
            setUniform1i(U_FLAGS, flags);
            flagsChanged = false;
        }
    }

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
    bool compile();
    bool link();
    void cacheAttribs();
    void cacheUniforms();

    GLuint programId;
    std::string vertSource;
    std::string fragSource;
    GLuint vertexShader;
    GLuint fragmentShader;
    std::unordered_map<std::string, int> attribs;
    std::unordered_map<std::string, int> uniforms;

    int flags = SHADER_FLAGS::STANDARD;
    bool flagsChanged = true;
};

}


#endif
