#include "glapp.h"
#include "lighting.h"
#include "cubes.h"

int main(int argc, char* argv[])
{
    GLApp* app;

    if (argc > 1) {
        int i = atoi(argv[1]);
        switch (i) {
        case 0:
            app = new Lighting;
            break;
        default:
            app = new Cubes;
            break;
        }
    }
    else {
        app = new Cubes;
    }
    app->init(640, 480);
    app->run();

    exit(EXIT_SUCCESS);
}
