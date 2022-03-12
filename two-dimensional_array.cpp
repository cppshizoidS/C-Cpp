#include <iostream>

int main()
{
    const int rows = 3, columns = 2;
    int numbers[rows][columns] = { {1, 2}, {3, 4}, {5, 6} };

    for(auto &subnumbers : numbers)
    {
        for(int number : subnumbers)
        {
            std::cout << number << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
