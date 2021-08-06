#include <iostream> //сортировка пузырьком
using namespace std;

int main()
{
    int* arr;   //указатель на переменную где юудет храниться массив
    int size;   //переменная с размером массива

    cout << "n = "; 
    cin >> size;    //ввод размера массива

    if (size <= 0) {
        cerr << "Invalid size" << endl;
        return 1;
    }

    arr = new int[size];


    for (int i = 0; i < size; i++) {
        cout << "arr[" << i << "] = ";
        cin >> arr[i];
    }

    int temp;

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {

                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }


    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;

    return 0;

}