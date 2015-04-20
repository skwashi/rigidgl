
#include "cubes.h"

int main(int argc, char* argv[])
{
    Cubes* cubes = new Cubes;
    cubes->init(640, 480);
    cubes->run();

    exit(EXIT_SUCCESS);
}
