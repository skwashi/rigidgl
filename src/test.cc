#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include "math/rm.h"
#include "math/rmath.h"
#include "math/tr3.h"
#include "camera.h"


using namespace std;
using namespace rm;
using namespace glm;

int main(int argc, char* argv[])
{
    // Mat3 m = Mat3(1) + Mat3(2);
    // Vec3 v = Vec3(0, 2, 1);
    
    // std::cout << m << std::endl;
    // std::cout << v << std::endl;
    // std::cout << m * v << std::endl;

    // quat q = angleAxis(R_PI/2, vec3(0, 0, 1));
    vec3 p = vec3(1, 0, 0);
    // quat q2 = angleAxis(R_PI/2, vec3(1, 0, 0));
    
    // vec3 p2 = p * q;
    // p = q * p * q;
    
    // cout << p.x << endl << p.y << endl << p.z << endl;
    // cout << p2.x << endl << p2.y << endl << p2.z << endl;


    quat q = angleAxis(R_PI/2 , glm::vec3(0, 1, 0));

    Tr3 tr = Tr3(q, 1, 0, 1);

    Tr3 tr2;
    vec3 dir = vec3(0, 0, -1);
    vec3 up = vec3(-1, 0, 0);

    tr2.setRotation(cross(dir, up), up, -dir);
    
    mat3 mat = mat3_cast(q);
    mat3 mat2;
    mat2[0] = vec3(0, 0, -1);
    mat2[1] = vec3(0, 1, 0);
    mat2[2] = vec3(1, 0, 0);
    

    Camera cam = Camera(45 * DEG_TO_RAD, 1, 1, 1000);

    cam.translate(0, 10, 100);
    cam.rotate(R_PI/4, 0, 1, 0);
    cout << to_string(cam.getPosition()) << endl;
    cam.translateLocally(0, 0, -50);
    cout << to_string(cam.getPosition()) << endl;

    return 0;
}
