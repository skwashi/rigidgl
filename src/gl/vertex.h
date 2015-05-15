/**
 * @file vertex.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-08
 */

#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

#include "vertexattrib.h"

namespace rgl {

struct Vertex2
{
    glm::vec2 pos;

    static const uint numFloats = 2;
    static const VertexAttribs& attribs;

    float& operator[](size_t i) { return ((float*) this)[i]; }
};

struct Vertex3
{
    glm::vec3 pos;

    static const uint numFloats = 3;
    static const VertexAttribs& attribs;
};

struct Vertex4
{
    glm::vec4 pos;

    static const uint numFloats = 4;
	static const VertexAttribs& attribs;
};

struct Vertex2n
{
    glm::vec2 pos;
    glm::vec3 n;

    static const uint numFloats = 5;
	static const VertexAttribs& attribs;
};

struct Vertex3n
{
    glm::vec3 pos;
    glm::vec3 n;

    static const uint numFloats = 6;
	static const VertexAttribs& attribs;
};

struct Vertex4n
{
    glm::vec4 pos;
    glm::vec3 n;

    static const uint numFloats = 7;
	static const VertexAttribs& attribs;
};

struct Vertex2t
{
    glm::vec2 pos;
    glm::vec2 t;

    static const uint numFloats = 4;
	static const VertexAttribs& attribs;
};

struct Vertex3t
{
    glm::vec3 pos;
    glm::vec2 t;

    static const uint numFloats = 5;
	static const VertexAttribs& attribs;
};

struct Vertex4t
{
    glm::vec4 pos;
    glm::vec2 t;

    static const uint numFloats = 6;
	static const VertexAttribs& attribs;
};

struct Vertex2c
{
    glm::vec2 pos;
    glm::vec4 c;

    static const uint numFloats = 6;
	static const VertexAttribs& attribs;
};

struct Vertex3c
{
    glm::vec3 pos;
    glm::vec4 c;

    static const uint numFloats = 7;
	static const VertexAttribs& attribs;
};

struct Vertex4c
{
    glm::vec4 pos;
    glm::vec4 c;

    static const uint numFloats = 8;
	static const VertexAttribs& attribs;
};

struct Vertex2nt
{
    glm::vec2 pos;
    glm::vec3 n;
    glm::vec2 t;

    static const uint numFloats = 7;
	static const VertexAttribs& attribs;
};

struct Vertex3nt
{
    glm::vec3 pos;
    glm::vec3 n;
    glm::vec2 t;

    static const uint numFloats = 8;
	static const VertexAttribs& attribs;
};

struct Vertex4nt
{
    glm::vec4 pos;
    glm::vec3 n;
    glm::vec2 t;

    static const uint numFloats = 9;
	static const VertexAttribs& attribs;
};

struct Vertex2nc
{
    glm::vec2 pos;
    glm::vec3 n;
    glm::vec4 c;

    static const uint numFloats = 9;
	static const VertexAttribs& attribs;
};

struct Vertex3nc
{
    glm::vec3 pos;
    glm::vec3 n;
    glm::vec4 c;

    static const uint numFloats = 10;
	static const VertexAttribs& attribs;
};

struct Vertex4nc
{
    glm::vec4 pos;
    glm::vec3 n;
    glm::vec4 c;

    static const uint numFloats = 11;
	static const VertexAttribs& attribs;
};

struct Vertex2tc
{
    glm::vec2 pos;
    glm::vec2 t;
    glm::vec4 c;

    static const uint numFloats = 8;
	static const VertexAttribs& attribs;
};

struct Vertex3tc
{
    glm::vec3 pos;
    glm::vec2 t;
    glm::vec4 c;

    static const uint numFloats = 9;
	static const VertexAttribs& attribs;
};

struct Vertex4tc
{
    glm::vec4 pos;
    glm::vec2 t;
    glm::vec4 c;

    static const uint numFloats = 10;
	static const VertexAttribs& attribs;
};

struct Vertex2ntc
{
    glm::vec2 pos;
    glm::vec3 n;
    glm::vec2 t;
    glm::vec4 c;

    static const uint numFloats = 11;
	static const VertexAttribs& attribs;
};

struct Vertex3ntc
{
    glm::vec3 pos;
    glm::vec3 n;
    glm::vec2 t;
    glm::vec4 c;

    static const uint numFloats = 12;
	static const VertexAttribs& attribs;
};

struct Vertex4ntc
{
    glm::vec4 pos;
    glm::vec3 n;
    glm::vec2 t;
    glm::vec4 c;

    static const uint numFloats = 13;
	static const VertexAttribs& attribs;
};

inline uint numFloats(const float& f) { return 1; }

inline uint numFloats(const glm::vec2& v) { return 2; }
inline uint numFloats(const glm::vec3& v) { return 3; }
inline uint numFloats(const glm::vec4& v) { return 4; }

inline uint numFloats(const Vertex2& v) { return 2; }
inline uint numFloats(const Vertex3& v) { return 3; }
inline uint numFloats(const Vertex4& v) { return 4; }

inline uint numFloats(const Vertex2n& v) { return 5; }
inline uint numFloats(const Vertex3n& v) { return 6; }
inline uint numFloats(const Vertex4n& v) { return 7; }

inline uint numFloats(const Vertex2t& v) { return 4; }
inline uint numFloats(const Vertex3t& v) { return 5; }
inline uint numFloats(const Vertex4t& v) { return 6; }

inline uint numFloats(const Vertex2c& v) { return 6; }
inline uint numFloats(const Vertex3c& v) { return 7; }
inline uint numFloats(const Vertex4c& v) { return 8; }

inline uint numFloats(const Vertex2nt& v) { return 7; }
inline uint numFloats(const Vertex3nt& v) { return 8; }
inline uint numFloats(const Vertex4nt& v) { return 9; }

inline uint numFloats(const Vertex2nc& v) { return 9; }
inline uint numFloats(const Vertex3nc& v) { return 10; }
inline uint numFloats(const Vertex4nc& v) { return 11; }

inline uint numFloats(const Vertex2tc& v) { return 8; }
inline uint numFloats(const Vertex3tc& v) { return 9; }
inline uint numFloats(const Vertex4tc& v) { return 10; }

inline uint numFloats(const Vertex2ntc& v) { return 11; }
inline uint numFloats(const Vertex3ntc& v) { return 12; }
inline uint numFloats(const Vertex4ntc& v) { return 13; }


inline const VertexAttribs& attribs(const glm::vec2& v) { return VAS_POS2; }
inline const VertexAttribs& attribs(const glm::vec3& v) { return VAS_POS3; }
inline const VertexAttribs& attribs(const glm::vec4& v) { return VAS_POS4; }

inline const VertexAttribs& attribs(const Vertex2& v) { return VAS_POS2; }
inline const VertexAttribs& attribs(const Vertex3& v) { return VAS_POS3; }
inline const VertexAttribs& attribs(const Vertex4& v) { return VAS_POS4; }

inline const VertexAttribs& attribs(const Vertex2n& v) { return VAS_2N; }
inline const VertexAttribs& attribs(const Vertex3n& v) { return VAS_3N; }
inline const VertexAttribs& attribs(const Vertex4n& v) { return VAS_4N; }

inline const VertexAttribs& attribs(const Vertex2t& v) { return VAS_2T; }
inline const VertexAttribs& attribs(const Vertex3t& v) { return VAS_3T; }
inline const VertexAttribs& attribs(const Vertex4t& v) { return VAS_4T; }

inline const VertexAttribs& attribs(const Vertex2c& v) { return VAS_2C; }
inline const VertexAttribs& attribs(const Vertex3c& v) { return VAS_3C; }
inline const VertexAttribs& attribs(const Vertex4c& v) { return VAS_4C; }

inline const VertexAttribs& attribs(const Vertex2nt& v) { return VAS_2NT; }
inline const VertexAttribs& attribs(const Vertex3nt& v) { return VAS_3NT; }
inline const VertexAttribs& attribs(const Vertex4nt& v) { return VAS_4NT; }

inline const VertexAttribs& attribs(const Vertex2nc& v) { return VAS_2NC; }
inline const VertexAttribs& attribs(const Vertex3nc& v) { return VAS_3NC; }
inline const VertexAttribs& attribs(const Vertex4nc& v) { return VAS_4NC; }

inline const VertexAttribs& attribs(const Vertex2tc& v) { return VAS_2TC; }
inline const VertexAttribs& attribs(const Vertex3tc& v) { return VAS_3TC; }
inline const VertexAttribs& attribs(const Vertex4tc& v) { return VAS_4TC; }

inline const VertexAttribs& attribs(const Vertex2ntc& v) { return VAS_2NTC; }
inline const VertexAttribs& attribs(const Vertex3ntc& v) { return VAS_3NTC; }
inline const VertexAttribs& attribs(const Vertex4ntc& v) { return VAS_4NTC; }

}

#endif
