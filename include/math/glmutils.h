/**
 * @author Jonas Ransjö
 */

#ifndef GLM_UTILS_H
#define GLM_UTILS_H

#include <glm/glm.hpp>

namespace glm {

inline mat3 mat3fromColumns(const vec3& col1, const vec3& col2, const vec3& col3)
    {
        mat3 mat;
        mat[0] = col1;
        mat[1] = col2;
        mat[2] = col3;
        return mat;
    }

inline mat4 mat4fromColumns(const vec3& col1, const vec3& col2, const vec3& col3, const vec3& col4)
    {
        mat4 mat;
        mat[0] = vec4(col1, 0);
        mat[1] = vec4(col2, 0);
        mat[2] = vec4(col3, 0);
        mat[3] = vec4(col4, 1);
        return mat;
    }


} //namespace

#endif
