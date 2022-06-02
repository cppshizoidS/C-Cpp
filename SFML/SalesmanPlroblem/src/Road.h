#pragma once
#include <SFML/Graphics.hpp>

#include "Town.h"

class Road
{
public:
	Road(Town *from, Town *to, sf::Color color);
	~Road();
	void draw(sf::RenderWindow &window);
	float distance();

private:
	sf::RectangleShape *rect;
	Town *t1;
	Town *t2;
	static float PI;
};