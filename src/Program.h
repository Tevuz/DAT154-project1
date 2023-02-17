#pragma once

#include <windows.h>
#include "GraphicEngine.h"

class Program {
public:
    Program();
    ~Program();

    void update(float delta);
    void input(WPARAM param);
    void render(GraphicEngine g);
};