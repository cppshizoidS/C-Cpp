#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Map.h"
#include "Road.h"

class Route
{
public:
	Route(Map &map, sf::Color color);
	void copyOrder(Route* dest);
	void swap(int i, int j);
	void drawRoads(sf::RenderWindow &window);
	void shuffle();
	void updateRoads();
	void setColor(sf::Color &color);
	float calcDistance();
	bool nextLexicOrder();
	~Route();

private:
	void reverse(int from);
	int townCount;
	int *order;
	sf::Color color;
	std::vector<Road *> roads;
	Map *map;
};