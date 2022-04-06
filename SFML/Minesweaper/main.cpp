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
            if (gridLogic[i - 1][j]==9) n++;
        }
    }
}
