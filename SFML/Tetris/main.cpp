#include <SFML/Graphics.hpp>
using namespace sf;

const int M = 20;   //height
const int N = 10;   //width

int manin(){
    RenderWindow window(VideoMode(320,400), "The Game");

    Texture texture;
    texture.loadFromFile("/home/cppshizoid/CLionProjects/Learning-Cpp/SFML/Tetris/images/tiles.png")

    Sprite sprite(texture);
    
    while (window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            //'X' bottom condition
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color::White);
        window.draw(sprite);
        window.display();
    }
    return 0;
}
