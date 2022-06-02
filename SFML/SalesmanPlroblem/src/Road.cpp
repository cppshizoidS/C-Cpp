#include <math.h>
#include "Road.h"

Road::Road(Town *from, Town *to, sf::Color color)
{
	this->t1 = from;
	this->t2 = to;
	float angle = atan2((t1->getY() - t2->getY()), (t1->getX() - t2->getX())) * 180 / PI - 180;
	this->rect = new sf::RectangleShape(sf::Vector2f(t1->distance(t2), 4));
	this->rect->setPosition(sf::Vector2f(t1->getX(), t1->getY()));
	this->rect->setFillColor(color);
	this->rect->setOrigin(0, 2);
	this->rect->setRotation(angle);
}

float Road::PI = 3.14159265f;

Road::~Road()
{
	delete this->rect;
}

void Road::draw(sf::RenderWindow &window)
{
	window.draw(sf::RectangleShape(*this->rect));
}

float Road::distance()
{
	return t1->distance(t2);
}