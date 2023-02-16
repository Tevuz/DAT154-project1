#pragma once

#include <iostream>
#include <vector>
#include "window.h"
#include "program.h"

/*class Car {

    const float VELOCITY_MAX = 1.0f;
    const float SPACING_MIN = 0.2f;

public:
    float position;
    float velocity;
    float length;

    void update(float delta, float nextCarPosition){
        float clearance = nextCarPosition - position - SPACING_MIN * (1.0 + velocity * velocity);

        velocity += clearance * delta;
        position += velocity * delta;

    }
};

enum TrafficLight {
    STOP, SLOW, GO, READY
};

class Road {
    std::vector<Car> cars;

    TrafficLight status;
    bool intersectionEmpty = true;

    const int INTERSECTION_START = -1.0f;
    const int INTERSECTION_END = 1.0f;


    void update(float delta) {
        intersectionEmpty = true;

        float nextCarPosition = 4.0f;

        for (auto car = cars.begin(); car != cars.end(); car++) {

            if (status == STOP && car->position < INTERSECTION_START)
                nextCarPosition = std::min(INTERSECTION_START, nextCarPosition);

            car->update(delta, nextCarPosition);
            nextCarPosition = car->position - car->length;
        }
    };
};

class Intersection {
    Road x;
    Road y;
};*/

int main() {
    std::cout << "Starting Application" << std::endl;

    Program* pProgram = new Program();
    Window* pWindow = new Window();
    pWindow->setRenderCallback([pProgram](){ pProgram->render();});
    pWindow->setUpdateCallback([pProgram](){ pProgram->update();});
    pWindow->start();
    pWindow->update();

    while (pWindow->ProcessMessages())
    {
        Sleep(50);
    }

    std::cout << "Closing Application" << std::endl;

    delete pWindow;
    //delete pProgram;

    return 0;
}
