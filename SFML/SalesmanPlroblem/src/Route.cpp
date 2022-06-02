#include <iostream>
#include <unistd.h>
#include <algorithm>

#include "Route.h"

Route::Route(Map &map, sf::Color color)
{
	this->map = &map;
	this->townCount = map.getTownCount();
	this->order = new int[townCount];
	this->color = color;
	for (int i = 0; i < townCount; ++i)
	{
		order[i] = i;
	}
	roads.reserve(townCount);
	for (int i = 0; i < townCount - 1; ++i)
	{
		roads.push_back(new Road(map.getTowns().at(order[i]), map.getTowns().at(order[i + 1]), color));
	}
	//Last road to get to starting town
	roads.push_back(new Road(map.getTowns().at(order[townCount - 1]), map.getTowns().at(order[0]), color));
}

void printOrder(int *arr, int size)
{
	for (int i = 0; i < size; ++i)
	{
		std::cout << arr[i];
	}
	std::cout << std::endl;
}

void Route::reverse(int from)
{
	std::reverse(order + from, order + townCount);
}

bool Route::nextLexicOrder()
{
	//printOrder(order, townCount);
	int X = -1;
	for (int i = 1; i < townCount - 1; ++i)
	{
		if (order[i] < order[i + 1])
			X = i;
	}
	if (X == -1)
		return true;
	int Y = -1;
	for (int i = 1; i < townCount; ++i)
	{
		if (order[X] < order[i])
			Y = i;
	}
	swap(X, Y);
	reverse(X + 1);
	return false;
}

void Route::copyOrder(Route *dest)
{
	for (int i = 0; i < townCount; ++i)
	{
		dest->order[i] = this->order[i];
	}
}

void Route::drawRoads(sf::RenderWindow &window)
{
	for (int i = 0; i < townCount; ++i)
	{
		roads.at(i)->draw(window);
	}
}

void Route::updateRoads()
{
	for (int i = 0; i < townCount - 1; ++i)
	{
		delete roads.at(i);
		roads.at(i) = new Road(map->getTowns().at(order[i]), map->getTowns().at(order[i + 1]), this->color);
	}
	delete roads.at(townCount - 1);
	roads.at(townCount - 1) = new Road(map->getTowns().at(order[townCount - 1]), map->getTowns().at(order[0]), color);
}

void Route::swap(int i, int j)
{
	int temp = order[i];
	order[i] = order[j];
	order[j] = temp;
}

//This shuffle function keeps first town always at the first town
void Route::shuffle()
{
	srand(time(NULL));
	for (int i = townCount - 1; i > 1; --i)
	{
		int j = rand() % i + 1;
		swap(i, j);
	}
}

float Route::calcDistance()
{
	float d = 0;
	for (int i = 0; i < townCount - 1; ++i)
	{
		d += map->getTowns().at(order[i])->distance(map->getTowns().at(order[i + 1]));
	}
	d += map->getTowns().at(order[townCount - 1])->distance(map->getTowns().at(order[0]));
	return d;
}

Route::~Route()
{
	for (unsigned int i = 0; i < roads.size(); ++i)
	{
		delete roads.at(i);
	}
}