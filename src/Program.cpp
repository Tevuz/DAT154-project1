#pragma once

#include "Program.h"
#include <iostream>

Program::Program()
{

}

Program::~Program()
{

}

void Program::update()
{
    std::cout << "Update" << std::endl;
}

void Program::render()
{
    std::cout << "Render" << std::endl;
}
