#pragma once

#include <windows.h>
#include "GraphicEngine.h"
#include "Keymap.h"

using Event = TrafficApp::Event;

class Program {
public:
    Program();
    ~Program();

    void update();
    void input(Event e);
    void render(GraphicEngine g);
};