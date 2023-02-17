#pragma once

#include <windows.h>
#include "GraphicEngine.h"

using Event = TrafficApp::Event;

class Program {
public:
    Program();
    ~Program();

    void update(float delta);
    void input(WPARAM param);
    void render(GraphicEngine g);
};