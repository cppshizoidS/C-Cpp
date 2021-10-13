#include <iostream>
#include <cstdlib> // для функций rand() и srand()

int main()
{
    srand(4541); // устанавливаем стартовое значение - 4 541

    // Выводим 100 случайных чисел
    for (int count = 0; count < 100; ++count)
    {
        std::cout << rand() << "\t";

        // Если вывели 5 чисел, то вставляем символ новой строки
        if ((count + 1) % 5 == 0)
            std::cout << "\n";
    }
}