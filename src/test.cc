#include "glapp.h"
#include "lighting.h"
#include "cubes.h"
#include "gamma.h"
#include "modelviewer.h"
#include "cubemaps.h"

int main(int argc, char* argv[])
{
    GLApp* app;

    if (argc > 1) {
        int i = atoi(argv[1]);
        switch (i) {
        case 0:
            app = new Cubes;
            break;
        case 1:
            app = new ModelViewer;
            break;
        case 2:
            app = new CubeMaps;
            break;
        case 3:
            app = new Gamma;
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
