#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1000
#define FRAMERATE 60
#define CITIES 20
#define COLOR_DENSITY 20
#define CONSOLE_OUTPUT 1
#define ITERATIONS 20

class AntAlgorithm
{
    template<typename T> using vector = std::vector<T>;

    struct CityType
    {
        int x, y;
    };

    struct AntType
    {
        int currentCity;
        int nextCity;
        vector<bool> usedCities;
        vector<int> path;
        double pathLength;

        void Restart(size_t newCity, int maxCities)
        {
            nextCity = -1;
            pathLength = 0.0;
            currentCity = newCity;
            path.clear();
            path.push_back(newCity);
            usedCities.clear();
            usedCities.resize(maxCities, false);
            usedCities[newCity] = true;
        }
    };
