/**
 * @file module.h
 *
 * @author Jonas Ransjö
 * @date 2015-05-10
 */

#ifndef MODULE_H
#define MODULE_H

#include "glapp.h"

class GLApp;

class Module
{
public:
    virtual void init(const GLApp* app);
    virtual void handleInput();
    virtual void update();
    virtual void render();
    bool isEnabled() { return enabled; }
    void enable() { enabled = true; }
    void disable() { enabled = true; }

    GLApp* app;
    bool enabled = true;
};

#endif
