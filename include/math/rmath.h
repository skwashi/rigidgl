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

} //namespace


#endif
