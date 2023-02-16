#include "Program.h"
#include <iostream>
#include <vector>

struct Car
{
    float pos;
    float vel;
    float acc;
    float len;
};

const int STOP = 0;
const int READY = 1;
const int GO = 2;
const int YIELD = 3;
struct TrafficLight
{

    int state = STOP;
};

const float BEGIN_POSITION = -1.0;
const float END_POSITION = 1.0;
const float LIGHT_POSITION = -0.2;
const float MIN_DISTANCE = 0.05;

std::vector<Car>* x_cars;
std::vector<Car>* y_cars;

TrafficLight* x_light;
TrafficLight* y_light;

Program::Program()
{
    x_cars = new std::vector<Car>;
    y_cars = new std::vector<Car>;
    x_light = new TrafficLight();
    y_light = new TrafficLight();

    x_cars->push_back({BEGIN_POSITION, 0, 0, 0});
    y_cars->push_back({BEGIN_POSITION, 0, 0, 0});
}

Program::~Program()
{
    delete x_light;
    delete y_light;
    delete[] x_cars;
    delete[] y_cars;
}

float updateCar(Car* car, TrafficLight light, float pos_next, float delta)
{
    if (car->pos < LIGHT_POSITION && light.state != GO)
        pos_next = LIGHT_POSITION;

    float distance = pos_next - car->pos - MIN_DISTANCE;
    distance *= 0.3f;
    distance = distance < 0.0f ? 0.0f : distance;
    distance = distance > 1.0f ? 1.0f : distance;
    car->acc += distance * (1.0f - car->acc) * delta;
    car->vel += car->acc * delta;
    car->pos += car->vel * delta;

    std::cout << car->vel << std::endl;

    return car->pos - car->len;
}

void Program::update()
{
    float delta = 1.0f / 60.0f;
    float pos_next;

    pos_next = END_POSITION;
    for (auto car = x_cars->begin(); car != x_cars->end(); car++)
        pos_next = updateCar(&*car, *x_light, pos_next, delta);

    pos_next = END_POSITION;
    for (auto car = y_cars->begin(); car != y_cars->end(); car++)
        pos_next = updateCar(&*car, *y_light, pos_next, delta);

}

void Program::render(GraphicEngine g)
{
    const int ROAD_WIDTH = 200;
    const int ROAD_STRIPE_LENGTH = 80;
    const int ROAD_STRIPE_WIDTH = 20;
    const int CAR_WIDTH = 50;
    const int CAR_LENGTH= 100;

    g.setLineColorDisabled();

    // ground
    g.setFillColor(96, 160, 32);
    g.fillRect(0, 0, g.width, g.height);

    // roads
    g.setFillColor(96, 96, 96);
    g.fillRect((g.width >> 1) - (ROAD_WIDTH >> 1) - 1, 0, ROAD_WIDTH, g.height);
    g.fillRect(0, (g.height >> 1) - (ROAD_WIDTH >> 1) - 1, g.width, ROAD_WIDTH);

    g.setFillColor(255, 255, 255);
    int len;
    len = (g.height - ROAD_WIDTH) / 2;
    for (int i = 0; i <  len / ROAD_STRIPE_LENGTH + 1; i+=2) {
        g.fillRect((g.width >> 1) - (ROAD_STRIPE_WIDTH >> 1) - 1, len - (i + 1) * ROAD_STRIPE_LENGTH, ROAD_STRIPE_WIDTH, ROAD_STRIPE_LENGTH);
        g.fillRect((g.width >> 1) - (ROAD_STRIPE_WIDTH >> 1) - 1, len + ROAD_WIDTH + i * ROAD_STRIPE_LENGTH, ROAD_STRIPE_WIDTH, ROAD_STRIPE_LENGTH);
    }
    len = (g.width - ROAD_WIDTH) / 2;
    for (int i = 0; i <  len / ROAD_STRIPE_LENGTH + 1; i+=2) {
        g.fillRect(len - (i + 1) * ROAD_STRIPE_LENGTH, (g.height >> 1) - (ROAD_STRIPE_WIDTH >> 1) - 1, ROAD_STRIPE_LENGTH, ROAD_STRIPE_WIDTH);
        g.fillRect(len + ROAD_WIDTH + i * ROAD_STRIPE_LENGTH, (g.height >> 1) - (ROAD_STRIPE_WIDTH >> 1) - 1, ROAD_STRIPE_LENGTH, ROAD_STRIPE_WIDTH);
    }

    // cars
    g.setFillColor(255, 64, 64);
    for (auto car = x_cars->begin(); car != x_cars->end(); car++)
        g.fillRect((g.width >> 1) + car->pos * 400.0, (g.height >> 1) - (ROAD_WIDTH >> 2) - (CAR_WIDTH >> 1) - (ROAD_STRIPE_WIDTH >> 2), CAR_LENGTH, CAR_WIDTH);

    g.setFillColor(64, 64, 255);
    for (auto car = y_cars->begin(); car != y_cars->end(); car++)
        g.fillRect((g.width >> 1) - (ROAD_WIDTH >> 2) - (CAR_WIDTH >> 1) - (ROAD_STRIPE_WIDTH >> 2), (g.height >> 1) + car->pos * 400.0, CAR_WIDTH, CAR_LENGTH);


    // lights
}
