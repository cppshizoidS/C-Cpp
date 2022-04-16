#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "managers/application.hpp"


int main()
{
    int COLUMNS_NUMBER = 100;


    srand(time(NULL));

    WindowManager windowManager(1000, 600);
    EventManager eventManager(&windowManager);
    ColumnManager columnManager(COLUMNS_NUMBER, &windowManager, &eventManager);
    SortManager sortManager(&windowManager, &columnManager);


    while(windowManager.isRunning()){

        //Event Handling when not in a loop
        sf::Event event;
        while(windowManager.getRenderWindow()->pollEvent(event)){

            switch (event.type){

                case sf::Event::Closed: 
                    windowManager.terminateWindow();
                    break;

                case sf::Event::Resized:
                    if (windowManager.getSize().x < 400 || windowManager.getSize().y < 250) 
                        windowManager.setSize(400, 250);
                    break;


                case sf::Event::KeyPressed:

                    /* KEYBINDS:
                    *  [1] - QUICKSORT
                    *  [2] - BUBBLESORT
                    *  [3] - INSERTIONSORT
                    *  [4] - SELECTIONSORT
                    *  [5] - MERGESORT
                    *  [6] - RADIXSORT
                    * 
                    *  [R] - SHUFFLE
                    *  [G] - GENERATE NEW COLUMNS
                    *  [SPACE] - SORT
                    */

                    if(event.key.code == sf::Keyboard::R){
                        columnManager.shuffle();
                        sortManager.setSorted(false);
                    }

                    else if(event.key.code == sf::Keyboard::G){
                        columnManager.generate(sortManager.getSortType());
                        sortManager.setSorted(false);
                    }

                    else if(event.key.code == sf::Keyboard::Space) sortManager.sort();

                    else if(event.key.code == sf::Keyboard::Num1) sortManager.setSortType(0);
                    else if(event.key.code == sf::Keyboard::Num2) sortManager.setSortType(1);
                    else if(event.key.code == sf::Keyboard::Num3) sortManager.setSortType(2);
                    else if(event.key.code == sf::Keyboard::Num4) sortManager.setSortType(3);
                    else if(event.key.code == sf::Keyboard::Num5) sortManager.setSortType(4);
                    else if(event.key.code == sf::Keyboard::Num6) sortManager.setSortType(5);
                    break;
                
                default:
                    break;
            }
        }

        columnManager.render();
    }

    return 0;
}   
