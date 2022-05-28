#include <iostream>
#include <vector>
#include <math.h>
#include <lib.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

long double calculateGravitationalForce(double mass1, double mass2, double distance) {
    const double GravitationalConstant = 6.674*pow(10.0, -11);
    const double force = GravitationalConstant*(mass1 * mass2 / pow(distance, 2));
    return force;
}

movementVector vectorFromPlanets(planet planet1, planet planet2) {
    movementVector newVector;
    newVector.x = planet2.cordinates[0] - planet1.cordinates[0];
    newVector.y = planet2.cordinates[1] - planet1.cordinates[1];
    return newVector;
}

movementVector getVectorFromForce(double mass, long double force, double direction) {
    movementVector newVector;
    double magnitude = (force / mass) / 100000 * 86400;
    newVector.setVector(direction, magnitude);
    return newVector;
}

double massToRadius(double mass) {
    double radius = cbrt(3 * (mass / (5514)) / (3 * M_PI));
    return radius;
}

int main() {
    srand (time(NULL));

    bool clearScreen = true;

    int screenDimensions[2] = {1500, 1000};
    const double pixelToSize = 3000000;
    bool mouseBtns[2];

    const int frameCap = 180;

    long int currentFrame = 0;
    long int currentPhysicsUpdate = 0;

    const int physicsUpdatesPerFrame = 40;

    int largestPlanetNum = 0;

    int planetAmt = 0;
    cout << "How many planets do you want? ";
    cin >> planetAmt;

    sf::RenderWindow window(sf::VideoMode(screenDimensions[0], screenDimensions[1]), "OggyP Gravity Sim!");
    window.setFramerateLimit(frameCap);
    sf::CircleShape planetShape(100.f);
    planetShape.setFillColor(sf::Color::Red);
    sf::CircleShape massRadius(100.f);
    massRadius.setFillColor(sf::Color::White);
    sf::VertexArray vectorDraw(sf::LinesStrip, 2);
    sf::RectangleShape fadeRect;
    fadeRect.setSize(sf::Vector2f(screenDimensions[0], screenDimensions[1]));
    fadeRect.setFillColor(sf::Color(0, 0, 0, 10));
    fadeRect.setPosition(sf::Vector2f(0, 0));

    vector <planet> planets;
    for (int i = 0; i < planetAmt; i++) {
        planet currentPlanet;
        largestPlanetNum++;
        currentPlanet.planetID = i;
        currentPlanet.cordinates[0] = rand() % screenDimensions[0] * pixelToSize;
        currentPlanet.cordinates[1] = rand() % screenDimensions[1] * pixelToSize;
        currentPlanet.mass = (rand() % 49 + 1) * pow(10, 24); // 1 to 100 * 10^24 | Earth is 5.9722 * 10^24
        planets.push_back(currentPlanet);
    }
    
    cout << largestPlanetNum << endl;
 
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // catch the resize events
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                screenDimensions[0] = event.size.width;
                screenDimensions[1] = event.size.height;
                fadeRect.setSize(sf::Vector2f(screenDimensions[0], screenDimensions[1]));
                window.setView(sf::View(visibleArea));
            }
        }

        mouseBtns[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        if (mouseBtns[0]) {
            int mouseCord[2] = {sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y};
            if (mouseCord[0] >= 0 && mouseCord[0] <= screenDimensions[0] && mouseCord[1] >= 0 && mouseCord[1] <= screenDimensions[1]) {
                planet newPlanet;
                newPlanet.planetID = largestPlanetNum;
                largestPlanetNum++;
                newPlanet.mass = 1 * pow(10, 24);
                newPlanet.cordinates[0] = mouseCord[0] * pixelToSize;
                newPlanet.cordinates[1] = mouseCord[1] * pixelToSize;
                while (mouseBtns[0]) {
                    window.clear();

                    newPlanet.mass += newPlanet.mass / frameCap;
                    double newPlanetRadius; 
                    double actualRadius;
                    bool customRadius = false;
                    if (newPlanet.customRadius != 0) {
                        newPlanetRadius = newPlanet.customRadius;
                        customRadius = true;
                        actualRadius = massToRadius(newPlanet.mass);
                    } else {
                        newPlanetRadius = massToRadius(newPlanet.mass);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                        newPlanet.customRadius = newPlanetRadius;
                    }

                    if (customRadius) {
                        massRadius.setRadius((float)(massToRadius(newPlanet.mass) / pixelToSize));
                        massRadius.setPosition(sf::Vector2f(newPlanet.cordinates[0] / pixelToSize - actualRadius / pixelToSize, newPlanet.cordinates[1] / pixelToSize - actualRadius / pixelToSize));    
                        window.draw(massRadius);
                    }

                    // draw all other planets
                    for (auto currentPlanet : planets) {
                        if (currentPlanet.isAlive) {
                            planetShape.setFillColor(sf::Color(currentPlanet.colour[0], currentPlanet.colour[1], currentPlanet.colour[2]));
                            double planetRadius;
                            if (currentPlanet.customRadius != 0) {
                                planetRadius = currentPlanet.customRadius;
                            } else {
                                planetRadius = massToRadius(currentPlanet.mass);
                            }
                            planetShape.setRadius((float)(planetRadius / pixelToSize));
                            planetShape.setPosition(sf::Vector2f(currentPlanet.cordinates[0] / pixelToSize - planetRadius / pixelToSize, currentPlanet.cordinates[1] / pixelToSize - planetRadius / pixelToSize));
                            window.draw(planetShape);
                        }
                    }

                    planetShape.setFillColor(sf::Color(newPlanet.colour[0], newPlanet.colour[1], newPlanet.colour[2]));
                    planetShape.setRadius((float)(newPlanetRadius / pixelToSize));
                    planetShape.setPosition(sf::Vector2f(newPlanet.cordinates[0] / pixelToSize - newPlanetRadius / pixelToSize, newPlanet.cordinates[1] / pixelToSize - newPlanetRadius / pixelToSize));
                    window.draw(planetShape);

                    mouseCord[0] = sf::Mouse::getPosition(window).x;
                    mouseCord[1] = sf::Mouse::getPosition(window).y;
                    vectorDraw[0].position = sf::Vector2f(mouseCord[0], mouseCord[1]);
                    vectorDraw[1].position = sf::Vector2f(newPlanet.cordinates[0] / pixelToSize, newPlanet.cordinates[1] / pixelToSize);
                    window.draw(vectorDraw);

                    window.display();

                    mouseBtns[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
                }
                mouseCord[0] = sf::Mouse::getPosition(window).x;
                mouseCord[1] = sf::Mouse::getPosition(window).y;

                newPlanet.vector.x = (mouseCord[0] * pixelToSize - newPlanet.cordinates[0]) / frameCap / physicsUpdatesPerFrame / 2; //move how far the mouse moved every 5 seconds
                newPlanet.vector.y = (mouseCord[1] * pixelToSize - newPlanet.cordinates[1]) / frameCap / physicsUpdatesPerFrame / 2;

                planets.push_back(newPlanet);

                window.clear();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            clearScreen = !clearScreen;
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {};
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            planets.clear();
            window.clear();
        }

        if (clearScreen) {
            if (currentFrame % frameCap == 0 or currentFrame % frameCap == round(frameCap / 2)) {
                window.draw(fadeRect);
            }
        } else {
            window.clear();
        }
        
        while (currentPhysicsUpdate < physicsUpdatesPerFrame) {
            for (auto &currentPlanet : planets) {
                if (currentPlanet.isAlive == false) continue; // do not run sim for dead planets
                double currentPlanetRadius = 0;
                if (currentPlanet.customRadius != 0) {
                    currentPlanetRadius = currentPlanet.customRadius;
                } else {
                    currentPlanetRadius = massToRadius(currentPlanet.mass);
                }
                for (auto &planetToCheck : planets) {
                    if (planetToCheck.planetID == currentPlanet.planetID || planetToCheck.isAlive == false) continue; // do not calculate for self
                    const float planetToCheckRadius = massToRadius(planetToCheck.mass);
                    movementVector vectorOfPlanets = vectorFromPlanets(currentPlanet, planetToCheck);
                    const double gravitationalForce = calculateGravitationalForce(currentPlanet.mass, planetToCheck.mass, vectorOfPlanets.getMagnitude());
                    movementVector vectorToAdd = getVectorFromForce(currentPlanet.mass, gravitationalForce, vectorOfPlanets.getDirection());
                    currentPlanet.vector.x += vectorToAdd.x;
                    currentPlanet.vector.y += vectorToAdd.y;
                    if (currentPlanetRadius + planetToCheckRadius > vectorOfPlanets.getMagnitude() - vectorToAdd.getMagnitude()) {
                        if (currentPlanet.mass > planetToCheck.mass) {
                            currentPlanet.mass += planetToCheck.mass;
                            planetToCheck.isAlive = false;
                        }
                    }
                }
            }
            for (auto &currentPlanet : planets) {
                currentPlanet.move();
            }
            currentPhysicsUpdate++;
        }

        currentPhysicsUpdate = 0;

        for (auto &currentPlanet : planets) {
            if (currentPlanet.isAlive == false) continue;
            double currentPlanetRadius = 0;
            if (currentPlanet.customRadius != 0) {
                currentPlanetRadius = currentPlanet.customRadius;
            } else {
                currentPlanetRadius = massToRadius(currentPlanet.mass);
            }
            planetShape.setRadius((float)(currentPlanetRadius / pixelToSize));
            planetShape.setFillColor(sf::Color(currentPlanet.colour[0], currentPlanet.colour[1], currentPlanet.colour[2]));
            planetShape.setPosition(sf::Vector2f(currentPlanet.cordinates[0] / pixelToSize - currentPlanetRadius / pixelToSize, currentPlanet.cordinates[1] / pixelToSize - currentPlanetRadius / pixelToSize));
            window.draw(planetShape);
        }

        window.display();
        currentFrame ++;
    }

    return 0;
}
