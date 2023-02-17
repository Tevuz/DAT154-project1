#include "Program.h"
#include <iostream>
#include <vector>
#include <ntdef.h>
#include <commctrl.h>
#include <queue>

struct Car
{
    float pos;
    float vel;
    float acc;
    float len;
    bool enabled = true;
};

const int STOP = 0;
const int READY = 1;
const int GO = 2;
const int YIELD = 3;
struct TrafficLight
{
    int state = STOP;
};

const int ROAD_WIDTH = 200;
const int ROAD_STRIPE_LENGTH = 80;
const int ROAD_STRIPE_WIDTH = 20;
const int CAR_WIDTH = 50;
const int CAR_LENGTH= 90;
const float CAR_SPEED= 150;
const float MIN_DISTANCE = 30.0;
const float STOPPING_DISTANCE = MIN_DISTANCE * 2.5f;

const float BEGIN_POSITION = -800.0;
const float END_POSITION = 800.0;
const float TARGET_POSITION = 2000.0;
const float LIGHT_POSITION = -ROAD_WIDTH;

float X_SPAWN_RATE = 0.1f;
float X_SPAWN_ACC = 0.0f;
float Y_SPAWN_RATE = 0.1f;
float Y_SPAWN_ACC = 0.0f;

std::deque<Car>* x_cars;
std::deque<Car>* y_cars;

TrafficLight* x_light;
TrafficLight* y_light;

Program::Program()
{
    x_cars = new std::deque<Car>;
    y_cars = new std::deque<Car>;
    x_light = new TrafficLight();
    x_light->state = GO;
    y_light = new TrafficLight();

    //x_cars->push_back({BEGIN_POSITION, 0, 0, 0});
    //y_cars->push_back({BEGIN_POSITION, 0, 0, 0});
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
    float target = pos_next - car->pos - MIN_DISTANCE;

    if (light.state != GO && car->pos < LIGHT_POSITION + MIN_DISTANCE && pos_next > LIGHT_POSITION + MIN_DISTANCE)
        target = LIGHT_POSITION - car->pos;

    car->pos += car->vel * delta;

    car->vel += car->acc * delta;
    car->vel = std::min(+CAR_SPEED, car->vel);
    car->vel = std::max(-CAR_SPEED, car->vel);

    float distance = abs(target);

    float s = CAR_SPEED * std::min(1.0f, distance / STOPPING_DISTANCE);

    float f = distance < 5.0 ? -car->vel : s * target / distance - car->vel;

    car->acc = f * 6.0f;
    car->acc = std::min(+200.0f, car->acc);
    car->acc = std::max(-200.0f, car->acc);

    if (car->pos > END_POSITION)
        car->enabled = false;

    return car->pos - CAR_LENGTH;
}

void Program::update(float delta)
{
    float pos_next;

    // update cars x
    pos_next = TARGET_POSITION;
    for (auto car = x_cars->begin(); car != x_cars->end(); car++)
        pos_next = updateCar(&*car, *x_light, pos_next, delta);

    // update cars y
    pos_next = TARGET_POSITION;
    for (auto car = y_cars->begin(); car != y_cars->end(); car++)
        pos_next = updateCar(&*car, *y_light, pos_next, delta);

    Car car;

    // despawn cars x
    while (!x_cars->empty() && !x_cars->front().enabled)
        x_cars->pop_front();

    // spawn cars x
    if (x_cars->empty() || (car = x_cars->back(), car.pos - car.len - MIN_DISTANCE > BEGIN_POSITION))
    {
        X_SPAWN_ACC += ((float) rand() / (float) RAND_MAX)  * delta;
        if (X_SPAWN_ACC > 1.0 - X_SPAWN_RATE) {
            x_cars->push_back({BEGIN_POSITION, 0, 0, 0});
            X_SPAWN_ACC = 0.0f;
        }
    }

    // despawn cars y
    while (!y_cars->empty() && !y_cars->front().enabled)
        y_cars->pop_front();
    // spawn cars y
    if (y_cars->empty() || (car = y_cars->back(), car.pos - car.len - MIN_DISTANCE > BEGIN_POSITION)){
        Y_SPAWN_ACC += ((float) rand() / (float) RAND_MAX)  * delta;
        if (Y_SPAWN_ACC > 1.0 - Y_SPAWN_RATE) {
            y_cars->push_back({BEGIN_POSITION, 0, 0, 0});
            Y_SPAWN_ACC = 0.0f;
        }
    }
}

void Program::input(WPARAM param)
{
    x_light->state++;
    x_light->state &= 3;

    y_light->state++;
    y_light->state &= 3;
    // TODO: traffic light improvement
}

void Program::render(GraphicEngine g)
{
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
        g.fillRect((g.width >> 1) + car->pos, (g.height >> 1) + (ROAD_WIDTH >> 2) - (CAR_WIDTH >> 1) - (ROAD_STRIPE_WIDTH >> 2), CAR_LENGTH, CAR_WIDTH);

    g.setFillColor(64, 64, 255);
    for (auto car = y_cars->begin(); car != y_cars->end(); car++)
        g.fillRect((g.width >> 1) - (ROAD_WIDTH >> 2) - (CAR_WIDTH >> 1) - (ROAD_STRIPE_WIDTH >> 2), (g.height >> 1) + car->pos, CAR_WIDTH, CAR_LENGTH);


    // lights
    // TODO: lights graphic
}
