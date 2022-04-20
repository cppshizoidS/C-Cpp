#include "../component/Component.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    int r, g, b;
    cout << "First color" << endl;
    cout << "R:"; cin >> r;
    cout << "G:"; cin >> g;
    cout << "B:"; cin >> b;
    ColorWrapper color = ColorWrapper(r, g, b);
    color.print();
    cout << "What color to add?" << endl;
    cout << "R:"; cin >> r;
    cout << "G:"; cin >> g;
    cout << "B:"; cin >> b;
    color.mix(r, g, b);
    color.print();
    return 0;
}