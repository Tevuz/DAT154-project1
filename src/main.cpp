#pragma once

#include <iostream>
#include <vector>
#include "window.h"
#include "program.h"

int main() {
    std::cout << "Starting Application" << std::endl;

    Program* pProgram = new Program();
    Window* pWindow = new Window();

    pWindow->setRenderCallback([pProgram](auto hWnd){ pProgram->render(hWnd); });
    pWindow->setUpdateCallback([pProgram](){ pProgram->update(); });
    pWindow->setInputCallback([pProgram](auto e) { pProgram->input(e); });

    pWindow->start();

    while (pWindow->ProcessMessages()) {}

    std::cout << "Closing Application" << std::endl;

    delete pWindow;
    delete pProgram;

    return 0;
}
