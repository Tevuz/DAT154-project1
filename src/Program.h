#pragma once

#include <windows.h>
#include "GraphicEngine.h"

class Program {
public:
    Program();
    ~Program();

    void update();
    void render(GraphicEngine g);
};