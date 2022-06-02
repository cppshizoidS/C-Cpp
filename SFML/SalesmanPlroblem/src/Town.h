#pragma once
#include <SFML/Graphics.hpp>

class Town
{
public:
	Town(int x, int y, int index);
	~Town();
	void draw(sf::RenderWindow &window);
	float distance(Town *t);
	int getX();
	int getY();

private:
	float PI = 3.14159265f;
	int x, y, index;
	sf::CircleShape *circle;
};