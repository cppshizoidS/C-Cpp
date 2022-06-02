#pragma once
#include <vector>
#include <SFML/Graphics.hpp>


#include "Town.h"


class Map
{
public:
	Map(unsigned int townCount, sf::RenderWindow &window);

	float calculateDistance();
	void drawMap(sf::RenderWindow &window);
	int getTownCount();
	std::vector<Town *> getTowns();

	~Map();
private:
	unsigned int townCount;
	int randomNum(int min, int max);
	std::vector<Town *> towns;
};