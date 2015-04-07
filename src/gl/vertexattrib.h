/**
 * @author Jonas Ransjö
 */

#ifndef VERTEXATTRIB_H
#define VERTEXATTRIB_H

#include <string>
#include <vector>
#include <GL/glew.h>

namespace rgl {

    struct VertexAttrib
    {
        std::string name;
        GLuint location;
        int numComponents;
    };

    const std::string A_POSITION = "a_position";
    const std::string A_COLOR = "a_color";
    const std::string A_NORMAL = "a_normal";
    const std::string A_TEXCOORD = "a_texCoord";

    const std::string U_PROJMATRIX = "u_projMatrix";
    const std::string U_VIEWMATRIX = "u_viewMatrix";
    const std::string U_MODELMATRIX = "u_modelMatrix";
    const std::string U_PROJVIEWMATRIX = "u_projViewMatrix";
    const std::string U_MVPMATRIX = "u_mvpMatrix";

    const VertexAttrib VA_POS2 = {A_POSITION, 0, 2};
    const VertexAttrib VA_POS3 = {A_POSITION, 0, 3};
    const VertexAttrib VA_POS4 = {A_POSITION, 0, 4};
    const VertexAttrib VA_NORMAL = {A_POSITION, 1, 3};
    const VertexAttrib VA_TEXCOORD = {A_TEXCOORD, 2, 2};
    const VertexAttrib VA_COLOR = {A_COLOR, 3, 4};
}

#endif
