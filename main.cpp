#include <iostream>
#include <vector>

class Car {
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
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
