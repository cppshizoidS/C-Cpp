#include <iostream>
#include <cstdlib> // ��� ������� rand() � srand()

int main()
{
    srand(4541); // ������������� ��������� �������� - 4 541

    // ������� 100 ��������� �����
    for (int count = 0; count < 100; ++count)
    {
        std::cout << rand() << "\t";

        // ���� ������ 5 �����, �� ��������� ������ ����� ������
        if ((count + 1) % 5 == 0)
            std::cout << "\n";
    }
}