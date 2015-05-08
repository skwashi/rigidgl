/**
 * @file vertex.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-08
 */

#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

namespace rgl {

struct Vertex2
{
    glm::vec2 pos;
};

struct Vertex3
{
    glm::vec3 pos;
};

struct Vertex4
{
    glm::vec4 pos;
};

struct Vertex2n
{
    glm::vec2 pos;
    glm::vec3 n;
};

struct Vertex3n
{
    glm::vec3 pos;
    glm::vec3 n;
};

struct Vertex4n
{
    glm::vec4 pos;
    glm::vec3 n;
};

struct Vertex2t
{
    glm::vec2 pos;
    glm::vec2 t;
};

struct Vertex3t
{
    glm::vec3 pos;
    glm::vec2 t;
};

struct Vertex4t
{
    glm::vec4 pos;
    glm::vec2 t;
};

struct Vertex2c
{
    glm::vec2 pos;
    glm::vec4 c;
};

struct Vertex3c
{
    glm::vec3 pos;
    glm::vec4 c;
};

struct Vertex4c
{
    glm::vec4 pos;
    glm::vec4 c;
};

struct Vertex2nt
{
    glm::vec2 pos;
    glm::vec3 n;
    glm::vec2 t;
};

struct Vertex3nt
{
    glm::vec3 pos;
    glm::vec3 n;
    glm::vec2 t;
};

struct Vertex4nt
{
    glm::vec4 pos;
    glm::vec3 n;
    glm::vec2 t;
};

struct Vertex2nc
{
    glm::vec2 pos;
    glm::vec3 n;
    glm::vec4 c;
};

struct Vertex3nc
{
    glm::vec3 pos;
    glm::vec3 n;
    glm::vec4 c;
};

struct Vertex4nc
{
    glm::vec4 pos;
    glm::vec3 n;
    glm::vec4 c;
};

struct Vertex2tc
{
    glm::vec2 pos;
    glm::vec2 t;
    glm::vec4 c;
};

struct Vertex3tc
{
    glm::vec3 pos;
    glm::vec2 t;
    glm::vec4 c;
};

struct Vertex4tc
{
    glm::vec4 pos;
    glm::vec2 t;
    glm::vec4 c;
};

struct Vertex2ntc
{
    glm::vec2 pos;
    glm::vec3 n;
    glm::vec2 t;
    glm::vec4 c;
};

struct Vertex3ntc
{
    glm::vec3 pos;
    glm::vec3 n;
    glm::vec2 t;
    glm::vec4 c;
};

struct Vertex4ntc
{
    glm::vec4 pos;
    glm::vec3 n;
    glm::vec2 t;
    glm::vec4 c;
};

inline uint numFloats(const float& f) { return 1; }

inline uint numFloats(const glm::vec2& v) { return 2; }
inline uint numFloats(const glm::vec3& v) { return 3; }
inline uint numFloats(const glm::vec4& v) { return 4; }

inline uint numFloats(const Vertex2) { return 2; }
inline uint numFloats(const Vertex3) { return 3; }
inline uint numFloats(const Vertex4) { return 4; }

inline uint numFloats(const Vertex2n) { return 5; }
inline uint numFloats(const Vertex3n) { return 6; }
inline uint numFloats(const Vertex4n) { return 7; }

inline uint numFloats(const Vertex2t) { return 4; }
inline uint numFloats(const Vertex3t) { return 5; }
inline uint numFloats(const Vertex4t) { return 6; }

inline uint numFloats(const Vertex2c) { return 6; }
inline uint numFloats(const Vertex3c) { return 7; }
inline uint numFloats(const Vertex4c) { return 8; }

inline uint numFloats(const Vertex2nt) { return 7; }
inline uint numFloats(const Vertex3nt) { return 8; }
inline uint numFloats(const Vertex4nt) { return 9; }

inline uint numFloats(const Vertex2nc) { return 9; }
inline uint numFloats(const Vertex3nc) { return 10; }
inline uint numFloats(const Vertex4nc) { return 11; }

inline uint numFloats(const Vertex2tc) { return 8; }
inline uint numFloats(const Vertex3tc) { return 9; }
inline uint numFloats(const Vertex4tc) { return 10; }

inline uint numFloats(const Vertex2ntc) { return 11; }
inline uint numFloats(const Vertex3ntc) { return 12; }
inline uint numFloats(const Vertex4ntc) { return 13; }

}

#endif
