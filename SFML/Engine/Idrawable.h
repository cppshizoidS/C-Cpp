#ifndef PSEUDO3DENGINE_IDRAWABLE_H
#define PSEUDO3DENGINE_IDRAWABLE_H

#include <SFML/Graphics.hpp>

class Idrawable
{
public:
    virtual ~Idrawable() {};
    virtual void draw(sf::RenderTarget& window) = 0;
};


#endif //PSEUDO3DENGINE_IDRAWABLE_H
