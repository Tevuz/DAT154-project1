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

const float START_POSITION = -1.0;
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
}

Program::~Program()
{
    delete x_light;
    delete y_light;
    delete[] x_cars;
    delete[] y_cars;
}

float updateCar(Car car, TrafficLight light, float pos_next, float delta)
{
    if (car.pos < LIGHT_POSITION && light.state != GO)
        pos_next = LIGHT_POSITION;

    float distance = pos_next - car.pos - MIN_DISTANCE;
    distance *= 0.3f;
    distance = distance < 0.0f ? 0.0f : distance;
    distance = distance > 1.0f ? 1.0f : distance;
    car.acc += distance * (1.0f - car.acc) * delta;
    car.vel += (1.0 - car.vel) * car.acc * delta;
    car.pos += car.vel * delta;

    return car.pos - car.len;
}

void Program::update()
{
    float delta = 1.0f / 60.0;
    std::cout << "Update" << std::endl;
    float pos_next;

    pos_next = END_POSITION;
    for (auto car = x_cars->begin(); car != x_cars->end(); car++)
        pos_next = updateCar(*car, *x_light, pos_next, delta);

    pos_next = END_POSITION;
    for (auto car = y_cars->begin(); car != y_cars->end(); car++)
        pos_next = updateCar(*car, *y_light, pos_next, delta);

}

void Program::render(HWND hWnd)
{
    std::cout << "Render" << std::endl;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    HGDIOBJ hOrg = SelectObject(hdc, brush);
    Ellipse(hdc, 10, 10, 200, 200);
    SelectObject(hdc, hOrg);
    DeleteObject(brush);
    EndPaint(hWnd, &ps);
}
