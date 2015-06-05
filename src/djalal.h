/**
 * @file djalal.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-22
 */

#ifndef DJALAL_H
#define DJALAL_H

#include <glm/glm.hpp>
#include <functional>

#include "gl/rgl.h"

namespace djalal {

template <typename V, typename T>
int leastN(std::function<V(V)> g, V v0, T r, uint max)
{
    uint n = 0;
    V vn = v0;

    while (n <= max) {
        if (length(vn) > r)
            break;
        n++;
        vn = g(vn);
    }

    return n;
}

rgl::VBuffer<rgl::Vertex4> mandlebrot()
{
    auto vbuffer = rgl::VBuffer<rgl::Vertex4>();

    return vbuffer;
}

} // namespace

#endif
