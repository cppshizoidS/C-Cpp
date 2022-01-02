#include <iostream>     //Implement Selection Sort
using namespace std;

void swapping(int& a, int& b) {         //swap the content of a and b
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void display(int* array, int size) {
    for (int i = 0; i < size; i++)
        cout << array[i] << " ";
    cout << endl;
}

void selectionSort(int* array, int size) {
    int i, j, imin;
    for (i = 0; i < size; i++) {
        imim = i;
        for (j = i + 1; j < size; j++) 
            if (array[j] < array[imin])
                imin = j;
            //placing in correct position
            swap(array[i], array[imin]);
        }
    }

int main() {
    int n;
        cout << "Enter the number of elements: ";
        cin >> n;
    int arr[n];
        cout << "Enter elements: " << endl;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
        cout << "Array before sorting: ";
     display(arr, n);
     selectionSort(arr, n);
        cout << "Array after sorting: ";
     display(arr, n);
}
