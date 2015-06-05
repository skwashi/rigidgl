/**
 * @author Jonas Ransjö
 */

#ifndef R_MATH_H
#define R_MATH_H

#include <cmath>
#include <algorithm>
#include <float.h>
#include <math.h>

#define R_EPSILON FLT_EPSILON
#define R_PI 3.14159265359f
#define R_TAU 6.28318530718f
#define DEG_TO_RAD (R_PI / 180.0f)
#define RAD_TO_DEG (180.0f / R_PI)

namespace rm {

using std::min;
using std::max;
using std::sqrt;
using std::abs;
using std::atan2;

typedef unsigned int uint;

inline bool isZero(float x)
{
    return (x < R_EPSILON && x > -R_EPSILON);
}

inline bool isZero(double x)
{
    return (x < DBL_EPSILON && x > -DBL_EPSILON);
}

template <typename T>
inline T clamp(T val, T min, T max)
{
    if (val > max)
        return max;
    if (val < min)
        return min;
    return val;
}

template <typename T>
inline T nearest(T val, T* vals, size_t numVals)
{
    if (numVals < 1)
        return val;

    T best = vals[0];
    for (size_t i = 1; i < numVals; i++) {
        if (abs(vals[i] - val) < abs(best - val))
            best = vals[i];
    }

    return best;
}

} //namespace


#endif
