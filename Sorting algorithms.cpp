#include <iostream>                         //sorting algorithms
#include <cstdio>
#include <cstdlib>
using namespace std;

void swapping(int& a, int& b) {             //implement selection Sort
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void display(int* array, int size) {
    for (int i = 0; i < size; i++) {
        cout << array[i] << " " << endl;
        }
    }

void selectionSort(int* array, int size) {          //selection sort
    int i, j, imin = 0;
    for (i = 0; i < size; i++) {
        int imim = i;
        for (j = i + 1; j < size; j++)
            if (array[j] < array[imin])
                imin = j;
        swap(array[i], array[imin]);
    }
}


void bubbleSort(int* array, int size) {     //bubble sort
    {
        int i, j,temp,pass = 0;
        int a[10] = {10,2,0,14,43,25,18,1,5,45};
            cout << "Input list: \n";
        for (i = 0; i < 10; i++) {
            cout << a[i] << "\t";
    }
            cout << endl;
        for (i = 0; i < 10; i++) {
            for (j = i + 1; j < 10; j++)
            {
                if (a[j] < a[i]) {
                temp = a[i];

                a[i] = a[j];
                a[j] = temp;
                    }
                }
            pass++;
        }
    cout << "Sorted Element List:\n";
        for (i = 0; i < 10; i++) {
            cout << a[i] << "\t";
    }

    cout<< "\nNumber of passes taken to sort the list:" << pass << endl;
    }

}


int main() {                        //print of results
    const int n = 10;
    int arr[n];
    std::cout << "Enter 10 elements: \n " << endl;
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::cout << "Array before sorting:\n ";
    display(arr, n);
    selectionSort(arr, n);
    std::cout << "Array after sorting: \n";
    display(arr, n);
}
