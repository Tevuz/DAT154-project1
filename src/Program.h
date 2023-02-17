#pragma once

#include <windows.h>
#include "GraphicEngine.h"
#include "window.h"

class Program {
public:
    Program();
    ~Program();

    void update(float delta, WPARAM param);
    void input(WPARAM param);
    void render(GraphicEngine g);

private:
    void updatePhysics(float delta);
    void updateLights(float delta);
};