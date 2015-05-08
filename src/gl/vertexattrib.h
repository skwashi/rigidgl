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

typedef std::vector<VertexAttrib> VertexAttribs;

const std::string A_POSITION = "a_position";
const std::string A_COLOR = "a_color";
const std::string A_NORMAL = "a_normal";
const std::string A_TEXCOORD = "a_texCoord";

const std::string U_PROJMATRIX = "u_p";
const std::string U_VIEWMATRIX = "u_v";
const std::string U_MODELMATRIX = "u_m";
const std::string U_PROJVIEWMATRIX = "u_pv";
const std::string U_MODELVIEWMATRIX = "u_mv";
const std::string U_MVPMATRIX = "u_mvp";
const std::string U_NORMALMATRIX = "u_normal";
const std::string U_TEXTURESAMPLER = "u_textureSampler";

const VertexAttrib VA_POS2 = {A_POSITION, 0, 2};
const VertexAttrib VA_POS3 = {A_POSITION, 0, 3};
const VertexAttrib VA_POS4 = {A_POSITION, 0, 4};
const VertexAttrib VA_NORMAL = {A_NORMAL, 1, 3};
const VertexAttrib VA_TEXCOORD = {A_TEXCOORD, 2, 2};
const VertexAttrib VA_COLOR = {A_COLOR, 3, 4};

const VertexAttribs VAS_POS2 = {VA_POS2};
const VertexAttribs VAS_POS3 = {VA_POS3};
const VertexAttribs VAS_POS4 = {VA_POS4};

const VertexAttribs VAS_2 = {VA_POS2};
const VertexAttribs VAS_3 = {VA_POS3};
const VertexAttribs VAS_4 = {VA_POS4};

const VertexAttribs VAS_POSNORM = {VA_POS3, VA_NORMAL};
const VertexAttribs VAS_2N = {VA_POS2, VA_NORMAL};
const VertexAttribs VAS_3N = {VA_POS3, VA_NORMAL};
const VertexAttribs VAS_4N = {VA_POS4, VA_NORMAL};

const VertexAttribs VAS_POSTEX = {VA_POS3, VA_TEXCOORD};
const VertexAttribs VAS_2T = {VA_POS2, VA_TEXCOORD};
const VertexAttribs VAS_3T = {VA_POS3, VA_TEXCOORD};
const VertexAttribs VAS_4T = {VA_POS4, VA_TEXCOORD};

const VertexAttribs VAS_POSCOL = {VA_POS3, VA_COLOR};
const VertexAttribs VAS_2C = {VA_POS2, VA_COLOR};
const VertexAttribs VAS_3C = {VA_POS3, VA_COLOR};
const VertexAttribs VAS_4C = {VA_POS4, VA_COLOR};


const VertexAttribs VAS_POSNORMTEX = {VA_POS3, VA_NORMAL, VA_TEXCOORD};
const VertexAttribs VAS_2NT = {VA_POS2, VA_NORMAL, VA_TEXCOORD};
const VertexAttribs VAS_3NT = {VA_POS3, VA_NORMAL, VA_TEXCOORD};
const VertexAttribs VAS_4NT = {VA_POS4, VA_NORMAL, VA_TEXCOORD};

const VertexAttribs VAS_POSNORMCOL = {VA_POS3, VA_NORMAL, VA_COLOR};
const VertexAttribs VAS_2NC = {VA_POS2, VA_NORMAL, VA_COLOR};
const VertexAttribs VAS_3NC = {VA_POS3, VA_NORMAL, VA_COLOR};
const VertexAttribs VAS_4NC = {VA_POS4, VA_NORMAL, VA_COLOR};

const VertexAttribs VAS_POSTEXCOL = {VA_POS3, VA_TEXCOORD, VA_COLOR};
const VertexAttribs VAS_2TC = {VA_POS2, VA_TEXCOORD, VA_COLOR};
const VertexAttribs VAS_3TC = {VA_POS3, VA_TEXCOORD, VA_COLOR};
const VertexAttribs VAS_4TC = {VA_POS4, VA_TEXCOORD, VA_COLOR};

const VertexAttribs VAS_POSNORMTEXCOL = {VA_POS3, VA_NORMAL, VA_TEXCOORD, VA_COLOR};
const VertexAttribs VAS_2NTC = {VA_POS2, VA_NORMAL, VA_TEXCOORD, VA_COLOR};
const VertexAttribs VAS_3NTC = {VA_POS3, VA_NORMAL, VA_TEXCOORD, VA_COLOR};
const VertexAttribs VAS_4NTC = {VA_POS4, VA_NORMAL, VA_TEXCOORD, VA_COLOR};

}

#endif
