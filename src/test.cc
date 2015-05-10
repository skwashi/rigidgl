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
            app = new Cubes;
            break;
        default:
            app = new Lighting;
            break;
        }
    }
    else {
        app = new Lighting;
    }
    app->init(640, 480);
    app->run();

    exit(EXIT_SUCCESS);
}
