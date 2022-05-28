#include <math.h>

const double PIlib = 3.141592653589793238463;

class movementVector {
    public:
        double x = 0;
        double y = 0;

        double getMagnitude() {
            const double magnitudeFromVector = sqrt(pow(x, 2) + pow(y, 2));
            return magnitudeFromVector;
        }

        double getDirection() {
            double direction = atan(x / y);
            if (y == 0) {
                if (x > 0) {
                    direction = PIlib / 2;
                } else {
                    direction = -PIlib / 2;
                }
            }
            else if (y < 0) {
                if (x > 0) {
                    direction = PIlib / 2 + (PIlib / 2 + direction);
                } else {
                    direction = - (PIlib / 2) - (PIlib / 2 - direction);
                }
            }
            return direction;
        }

        void setVector(double direction, double magnitude) {
            x = magnitude * sin(direction);
            y = magnitude * cos(direction);
        }
};

class planet {
    public:
        movementVector vector;
        int colour[3] = {rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55};
        int planetID;
        bool isAlive = true;
        double mass; // in kg
        double customRadius = 0;
        double cordinates[2] = {0, 69}; // in km, will be divided by 3000000 to get pixels
        void move() { // moves the planet by the vector
            cordinates[0] += vector.x;
            cordinates[1] += vector.y;
        }
};
