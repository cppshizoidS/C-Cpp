#include <SFML/Grapics.hpp>
#include <time.h>
using namespace sf;

int main(){
    //random generator
    srand(time(0));

    RenderWindow app(VideoMode(400, 400), "Minesweaper");
    //width of cell
    int w =32;
    int gridLogic[12][12];
    int gridView[12][12];

    //texture and sprite
    Texture t;
    t.loadFromFile("images/titles.jpg");
    Sprite s(t);

    for (int i; i<= 10; i++){
        for (int j; j<= 10; j++){
            gridView[i][i] = 10;
            if (rand() % 5 ==0) gridLogic[i][j] = 9;
            else gridLogic[i][j] = 0;
        }
    }
    //count of mine around every cell
    for (int i = 1; i <=10; i++){
        for (int j = 1; j <=10; j++){
            int n = 0;
            if (gridLogic[i][j] == 9) continue;
            if (gridLogic[i+1][j] == 9) n++;
            if (gridLogic[i][j+1] == 9) n++;
            if (gridLogic[i-1][j] == 9) n++;
            if (gridLogic[i][j-1] == 9) n++;
            if (gridLogic[i+1][j+1] == 9) n++;
            if (gridLogic[i-1][j-1] == 9) n++;
            if (gridLogic[i-1][j+1] == 9) n++;
            if (gridLogic[i+1][j-1] == 9) n++;
            gridLogic[i][j] = n;
        }
        while (app.isOpen()){
            Vector2i pos = Mouse::getPosition(app);
            int x = pos.x / w;
            int y = pos.y / w;

            Event e;
            while (app.pollEvent(e)){
                if (e.type == Event::Closed)
                    app.close();

                //mouse click
                if (e.type == Event::MouseButtonPressed)
                    //if left - open a cell
                    if (e.key.code == Mouse::Left) gridView[x][y] = gridLogic[x][y];
                    //if right click - draw a flag
                    else if (e.key.code == Mouse::Right) gridView[x][y] == 11;
            }

            app.clear(Color::White);

            for (int i = 1; i<=10; i++)
                for (int j = 1; j <= 10; j++)
                {
                    if (gridView[x][y] == 9) gridView[i][j] = gridLogic[i][j];
                    //square from sprite
                    s.setTextureRect(IntRect(gridView[i][j] * w, 0, w, w));
                    // position
                    s.setPosition(i*w, j*w);
                    //draw
                    app.draw(s);
                }
            //display render
            app.display();
        }
    }
    return 0;
}
