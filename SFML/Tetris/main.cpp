#include <SFML/Graphics.hpp>
using namespace sf;

const int M = 20; // heigh
const int N = 10; // width 

int field[M][N] = { 0 }; // playing field

// figures of tetramino
int figures[7][4]=
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

struct Point
{
	int x, y;
} a[4], b[4];


int main()
{
	RenderWindow window(VideoMode(320, 480), "The Game!");

	// textures
	Texture texture;
	texture.loadFromFile("C:\\dev\\SFML_Tutorial\\images\\tiles.png");

	// sprite
	Sprite sprite(texture);
	
	//  18х18 pixels square
	sprite.setTextureRect(IntRect(0, 0, 18, 18));

	// while window open
	while (window.isOpen())
	{
		// events
		Event event;
		while (window.pollEvent(event))
		{
			// 'X' bottom
			if (event.type == Event::Closed)
				window.close();
		}

		int n = 3; // type of tetramino
		for (int i = 0; i < 4; i++)
		{
			a[i].x = figures[n][i] % 2;
			a[i].y = figures[n][i] / 2;
		}
	
		// background - white
		window.clear(Color::White);

		for (int i = 0; i < 4; i++)
		{
			// positions of tetraminos
			sprite.setPosition(a[i].x * 18, a[i].y * 18);
			
			// render of sprite
			window.draw(sprite);
		}
		// render of window
		window.display();
	}

	return 0;
}
