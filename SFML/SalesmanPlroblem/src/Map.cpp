#include "Map.h"

Map::Map(unsigned int townCount, sf::RenderWindow &window)
{
	srand(time(NULL));
	this->townCount = townCount;
	towns.reserve(townCount);
	for (unsigned int i = 0; i < townCount; ++i)
	{
		towns.push_back(new Town(randomNum(10, window.getSize().x - 10), randomNum(10, window.getSize().y - 10), i));
	}
}
void Map::drawMap(sf::RenderWindow &window)
{
	for(unsigned int i = 0; i < townCount; ++i)
	{
		towns.at(i)->draw(window);
	}
}

std::vector<Town *> Map::getTowns()
{
	return towns;
}

int Map::getTownCount()
{
	return this->townCount;
}

int Map::randomNum(int min, int max)
{
	return min + (std::rand() % (max - min + 1));
}

Map::~Map()
{
	for (int i = 0; i < townCount; ++i)
	{
		delete towns.at(i);
	}
	//delete towns;
}