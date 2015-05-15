/**
 * @author Jonas Ransjö
 */

#ifndef GLM_UTILS_H
#define GLM_UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <sstream>

#include "rtypes.h"

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

inline std::ostream &operator<< (std::ostream& out, const glm::vec2& v) {
    out << "("
        << v.x << " " << v.y
        << ")";

    return out;
}

inline std::ostream &operator<< (std::ostream& out, const glm::vec3& v) {
    out << "("
        << v.x << " " << v.y << " " << v.z
        << ")";

    return out;
}

inline std::ostream &operator<< (std::ostream& out, const glm::vec4& v) {
    out << "("
        << v.x << " " << v.y << " " << v.z << " " << v.w
        << ")";

    return out;
}

inline std::ostream &operator<< (std::ostream& out, const glm::quat& q) {
    out << "("
        << q.x << " " << q.y << " " << q.z << " " << q.w
        << ")";

    return out;
}


inline std::istream &operator>> (std::istream& is, glm::vec2& v) {
    is >> v.x; is >> v.y;
    return is;
}

inline std::istream &operator>> (std::istream& is, glm::vec3& v) {
    is >> v.x; is >> v.y; is >> v.z;
    return is;
}

inline std::istream &operator>> (std::istream& is, glm::vec4& v) {
    is >> v.x; is >> v.y; is >> v.z; is >> v.w;
    return is;
}

} //namespace

#endif
