/**
 * @author Jonas Ransjö
 */

#ifndef GLAPP_H
#define GLAPP_H

#include <GLFW/glfw3.h>

class GLApp
{
public:

    GLApp()
    {
    }

    void init();
    void run();
    void stop();

protected:

    enum State {
        RUNNING = 0,
        PAUSED = 1,
        STOP = 2
    };

    State state;

    double time; // time in seconds since init
    double prevTime; // time at last frame
    double dt; // time - prevTime
    int frameCount;
    int fps;
    int timeStamp;

    void initTime();
    void updateTime();
    void updateFPS();
    void prepare();
    void update();
    void render();
    void cleanUp();
    void destroy();

};

#endif
