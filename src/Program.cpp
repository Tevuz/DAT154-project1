#include "Program.h"
#include <iostream>
#include <vector>
#include <queue>
#include <sstream>

struct Car
{
    float pos;
    float vel;
    float acc;
    uint8_t col_id;
    bool enabled = true;
};

const int colors[] {0x00f08030, 0x0080f030, 0x00303030, 0x003080f0, 0x00f0f0f0, 0x003030f0, 0x0030f0f0, 0x00f03030};
const int num_colors = sizeof (colors) / sizeof(int);

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
const int LIGHT_WIDTH = 30;

float X_SPAWN_RATE = 0.1f;
float Y_SPAWN_RATE = 0.1f;

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
    y_light->state = STOP;

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
    if (x_cars->empty() || (car = x_cars->back(), car.pos - car.len - MIN_DISTANCE > BEGIN_POSITION)){
        if (((float) rand() / (float) RAND_MAX) < X_SPAWN_RATE  * delta) {
            x_cars->push_back({BEGIN_POSITION, 0, 0, (uint8_t) (rand() % num_colors), true});
        }
    }

    // despawn cars y
    while (!y_cars->empty() && !y_cars->front().enabled)
        y_cars->pop_front();
    // spawn cars y
    if (y_cars->empty() || (car = y_cars->back(), car.pos - car.len - MIN_DISTANCE > BEGIN_POSITION)){
        if (((float) rand() / (float) RAND_MAX) < Y_SPAWN_RATE  * delta) {
            y_cars->push_back({BEGIN_POSITION, 0, 0, (uint8_t) (rand() % num_colors), true});
        }
    }
}

void Program::input(WPARAM param)
{
    switch (param)
    {
        case VK_LBUTTON:
            x_light->state++;
            x_light->state &= 3;

            y_light->state++;
            y_light->state &= 3;
            break;
        case VK_LEFT:
            X_SPAWN_RATE = std::max(X_SPAWN_RATE - 0.1f, 0.0f);
            std::cout << "decreasing x spawn rate to " << X_SPAWN_RATE << std::endl;
            break;
        case VK_RIGHT:
            X_SPAWN_RATE = std::min(X_SPAWN_RATE + 0.1f, 1.0f);
            std::cout << "increasing x spawn rate to " << X_SPAWN_RATE << std::endl;
            break;
        case VK_UP:
            Y_SPAWN_RATE = std::min(Y_SPAWN_RATE + 0.1f, 1.0f);
            std::cout << "increasing y spawn rate to " << Y_SPAWN_RATE << std::endl;
            break;
        case VK_DOWN:
            Y_SPAWN_RATE = std::max(Y_SPAWN_RATE - 0.1f, 0.0f);
            std::cout << "decreasing y spawn rate to" << Y_SPAWN_RATE << std::endl;
            break;
    }
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
    for (auto car = x_cars->begin(); car != x_cars->end(); car++){
        g.setFillColor(colors[car->col_id]);
        g.fillRect((g.width >> 1) + car->pos, (g.height >> 1) + (ROAD_WIDTH >> 2) - (CAR_WIDTH >> 1) - (ROAD_STRIPE_WIDTH >> 2), CAR_LENGTH, CAR_WIDTH);
    }

    g.setFillColor(64, 64, 255);
    for (auto car = y_cars->begin(); car != y_cars->end(); car++) {
        g.setFillColor(colors[car->col_id]);
        g.fillRect((g.width >> 1) - (ROAD_WIDTH >> 2) - (CAR_WIDTH >> 1) - (ROAD_STRIPE_WIDTH >> 2), (g.height >> 1) + car->pos, CAR_WIDTH, CAR_LENGTH);
    }


    // lights

    // light y
    g.setFillColor(32, 32, 32);
    g.fillRect((g.width >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH), (g.height >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH * 3), LIGHT_WIDTH, LIGHT_WIDTH * 3);

    if (y_light->state == STOP || y_light->state == READY)
        g.setFillColor(255, 0, 0);
    else
        g.setFillColor(64, 0, 0);
    g.fillEllipse((g.width >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH), (g.height >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH * 1), LIGHT_WIDTH, LIGHT_WIDTH);
    if (y_light->state == YIELD || y_light->state == READY)
        g.setFillColor(255, 128, 0);
    else
        g.setFillColor(64, 32, 0);
    g.fillEllipse((g.width >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH), (g.height >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH * 2), LIGHT_WIDTH, LIGHT_WIDTH);
    if (y_light->state == GO)
        g.setFillColor(0, 255, 0);
    else
        g.setFillColor(0, 64, 0);
    g.fillEllipse((g.width >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH), (g.height >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH * 3), LIGHT_WIDTH, LIGHT_WIDTH);

    // light x
    g.setFillColor(32, 32, 32);
    g.fillRect((g.width >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH*3), (g.height >> 1) + (ROAD_WIDTH >> 1) - 2, LIGHT_WIDTH * 3, LIGHT_WIDTH);

    if (x_light->state == STOP || x_light->state == READY)
        g.setFillColor(255, 0, 0);
    else
        g.setFillColor(64, 0, 0);
    g.fillEllipse((g.width >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH*1), (g.height >> 1) + (ROAD_WIDTH >> 1) - 2, LIGHT_WIDTH, LIGHT_WIDTH);

    if (x_light->state == YIELD || x_light->state == READY)
        g.setFillColor(255, 128, 0);
    else
        g.setFillColor(64, 32, 0);
    g.fillEllipse((g.width >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH*2), (g.height >> 1) + (ROAD_WIDTH >> 1) - 2, LIGHT_WIDTH, LIGHT_WIDTH);

    if (x_light->state == GO)
        g.setFillColor(0, 255, 0);
    else
        g.setFillColor(0, 64, 0);
    g.fillEllipse((g.width >> 1) - (ROAD_WIDTH >> 1) - (LIGHT_WIDTH*3), (g.height >> 1) + (ROAD_WIDTH >> 1) - 2, LIGHT_WIDTH, LIGHT_WIDTH);

    // text
    std::ostringstream oss;

    oss << "Number of cars: " << (x_cars->size() + y_cars->size()) << "(North: " << (y_cars->size()) << ", West: " << (x_cars->size()) << ")";
    g.drawText(oss.str(), 0, 0);

    oss.str("");
    oss.clear();
    oss << "Probability car spawn from north: " << Y_SPAWN_RATE * 100 << "%";
    g.drawText(oss.str(), 0, 20);

    oss.str("");
    oss.clear();
    oss << "Probability car spawn from west: " << X_SPAWN_RATE * 100 << "%";
    g.drawText(oss.str(), 0, 40);
}
