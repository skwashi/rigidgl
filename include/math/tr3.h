/**
 * @author Jonas Ransjö 
 */

#ifndef TR3_H
#define TR3_H

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "rmath.h"
#include "glmutils.h"

namespace rm {

    struct Tr3 
    {
        glm::quat q;
        glm::vec3 p;
        
        Tr3() {}

        Tr3(const glm::vec3& _p)
        {
            p = _p;
        }

        Tr3(const glm::quat& _q, const glm::vec3& _p)
        {
            q = _q;
            p = _p;
        }

        Tr3(float x, float y, float z)
        {
            p = glm::vec3(x, y ,z);
        }

        Tr3(const glm::quat& _q, float x, float y, float z)
        {
            q = _q;
            p = glm::vec3(x, y ,z);
        }

        Tr3(const glm::quat& _q)
        {
            q = _q;
        }

        Tr3(const Tr3& tr)
        {
            q = tr.q;
            p = tr.p;
        }

        void setRotation(const glm::quat& _q)
        {
            q = _q;
        }

        void setRotation(const glm::vec3& axis1, const glm::vec3& axis2, const glm::vec3& axis3) 
        {
            glm::mat3 mat = glm::mat3fromColumns(axis1, axis2, axis3);
            q = glm::quat_cast(mat);
        }
        
        void setRotation(float angle, const glm::vec3& axis)
        {
            q = glm::angleAxis(angle, axis);
        }

        void setRotation(float angle, float x, float y, float z)
        {
            q = glm::angleAxis(angle, glm::vec3(x, y, z));
        }

        void setPosition(const glm::vec3& _p)
        {
            p = _p;
        }

        void setPosition(float x, float y, float z)
        {
            p = glm::vec3(x, y, z);
        }

        void set(const Tr3& tr)
        {
            q = tr.q;
            p = tr.p;
        }

        void setIdentity()
        {
            q = glm::quat();
            p = glm::vec3();
        }

        void normalize()
        {
            q = glm::normalize(q);
        }

        void addRotation(const glm::quat& _q)
        {
            q = _q * q;
        }

        void addRotation(float angle, const glm::vec3& axis)
        {
            q = rotate(q, angle, axis);
        }

        void addRotation(float angle, float x, float y, float z)
        {
            q = rotate(q, angle, glm::vec3(x, y, z));
        }
        
        void addLocalRotation(const glm::quat& _q) 
        {
            q *= _q;
        }        

        void addLocalRotation(float angle, const glm::vec3& axis)
        {
            q = glm::angleAxis(angle, q * axis) * q;
        }

        void addLocalRotation(float angle, float x, float y, float z)
        {
            q = glm::angleAxis(angle, q * glm::vec3(x, y, z)) * q;
        }
        
        void addTranslation(const glm::vec3& _p) 
        {
            p += _p;
        }

        void addTranslation(float x, float y, float z) 
        {
            p += glm::vec3(x, y, z);
        }

        void addLocalTranslation(const glm::vec3& _p)
        {
            p += q * _p;
        }

        void addLocalTranslation(float x, float y, float z)
        {
            p += q * glm::vec3(x, y, z);
        }
        
        void rotateAround(const glm::vec3& pivot, const glm::quat& _q) 
        {
            p = _q * (p - pivot) + pivot;
            q = _q * q;
        }
        
        void rotateAround(const glm::vec3& pivot, float angle, const glm::vec3& axis) 
        {
            rotateAround(pivot, glm::angleAxis(angle, axis));
        }

        void rotateLocallyAround(const glm::vec3& pivot, float angle, const glm::vec3& axis) 
        {
            rotateAround(pivot, glm::angleAxis(angle, q * axis));
        }
        
        void invert() 
        {
            q = glm::conjugate(q);
            p = -(q * p);
        }
        
        void operator *= (const Tr3& tr) 
        {
            p += q * tr.p;
            q *= tr.q;
        }
        
        void operator *=(const glm::quat& _q)
        {
            q = _q * q;
        }

        void operator += (const glm::vec3& _p) {
            p += q * _p;
        }

        void operator -= (const glm::vec3& _p) {
            p -= q * _p;
        }

        /* Vector multiplication */

        glm::vec3 mult(const glm::vec3& v) 
        {
            return q * v + p;
        }

        glm::vec3 multI(const glm::vec3& v) 
        {
            return (v - p) * q;
        }

        glm::vec3 multV(const glm::vec3& v) 
        {
            return q * v;
        }

        glm::vec3 multIV(const glm::vec3& v) 
        {
            return v * q;
        }

        glm::vec3 operator *(const glm::vec3& v) 
        {
            return q * v + p;
        }

        /* Matrix representation */

        glm::mat4 toMat4() const
        {
            glm::mat4 mat = glm::mat4_cast(q);
            mat[3][0] = p.x;
            mat[3][1] = p.y;
            mat[3][2] = p.z;
            return mat;
        }

        glm::mat4 toMat4I() const
        {
            glm::quat qi = glm::conjugate(q);
            glm::vec3 pi = -(qi * p);
            glm::mat4 mat = glm::mat4_cast(qi);
            mat[3][0] = pi.x;
            mat[3][1] = pi.y;
            mat[3][2] = pi.z;
            return mat;
        }
        
    };
    
    /* Operators */

    inline Tr3 inverse(const Tr3& tr) 
    {
        glm::quat q = glm::conjugate(tr.q);
        glm::vec3 p = -(q * tr.p);
        return Tr3(q, p);
    }

    inline Tr3 mult(const Tr3& trA, const Tr3& trB) 
    {
        glm::quat q = trA.q * trB.q;
        glm::vec3 p = trA.q * trB.p + trA.p;
        return Tr3(q, p);
    }

    inline Tr3 multIA(const Tr3& trA, const Tr3& trB) 
    {
        glm::quat aqi = glm::conjugate(trA.q);
        glm::quat q = aqi * trB.q;
        glm::vec3 p = aqi * (trB.p - trA.p);
        return Tr3(q, p);
    }

    inline Tr3 multIB(const Tr3& trA, const Tr3& trB) 
    {
        glm::quat bqi = glm::conjugate(trB.q);
        glm::quat q = trA.q * bqi;        
        glm::vec3 p = trA.p - q * trB.p;
        return Tr3(q, p);
    }

    inline Tr3 multIAB(const Tr3& trA, const Tr3& trB) 
    {
        glm::quat aqi = glm::conjugate(trA.q);
        glm::quat bqi = glm::conjugate(trB.q);
        glm::quat q = aqi * bqi;
        glm::vec3 p = -(aqi * (bqi * trB.p + trA.p));
        return Tr3(q, p);
    }
    
    inline Tr3 operator *(const Tr3& trA, const Tr3& trB) 
    {
        return mult(trA, trB);
    }
    
    
    /* Vector multiplication */

    inline glm::vec3 mult(const Tr3& tr, const glm::vec3& p) 
    {
        return tr.q * p + tr.p;
    }
    
    inline glm::vec3 multI(const Tr3& tr, const glm::vec3 p) 
    {
        return (p - tr.p) * tr.q;
    }

    inline glm::vec3 multV(const Tr3& tr, const glm::vec3 p)
    {
        return tr.q * p;
    }

    inline glm::vec3 multIV(const Tr3& tr, const glm::vec3 p)
    {
        return p * tr.q;
    }

    inline glm::vec3 operator *(const Tr3& tr, const glm::vec3& p) 
    {
        return tr.q * p + tr.p;
    }

    inline glm::vec3 operator *(const glm::vec3 p, const Tr3& tr) 
    {
        return (p - tr.p) * tr.q;
    }

    
    /* Matrix representation */

    inline glm::mat4 toMat4(const Tr3& tr)
    {
        glm::mat4 mat = glm::mat4_cast(tr.q);
        mat[3][0] = tr.p.x;
        mat[3][1] = tr.p.y;
        mat[3][2] = tr.p.z;
        return mat;
    }

    inline glm::mat4 toMat4I(const Tr3& tr)
    {
        glm::quat q = glm::conjugate(tr.q);
        glm::vec3 p = -(q * tr.p);
        glm::mat4 mat = glm::mat4_cast(q);
        mat[3][0] = p.x;
        mat[3][1] = p.y;
        mat[3][2] = p.z;
        return mat;
    }

    
    
} //namespace

#endif
