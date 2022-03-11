#include <iostream>                         //sorting algorithms
#include <cstdio>
using namespace std;

void swapping(int& a, int& b) {             //implement Selection Sort
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

void selectionSort(int* array, int size) {          //selection sort
    int i, j, imin;
    for (i = 0; i < size; i++) {
        imim = i;
        for (j = i + 1; j < size; j++) 
            if (array[j] < array[imin])
                imin = j;
            swap(array[i], array[imin]);
        }
    }


void bubbleSort(int* array, int size) {     //bubble sort
    int i, j,
    {
        int i, j,temp,pass = 0;
        int a[10] = {10,2,0,14,43,25,18,1,5,45};
            cout << "Input list ...\n";
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
    cout << "Sorted Element List ...\n";
    for (i = 0; i < 10; i++) {
        cout << a[i] << "\t";
    };
    cout << "\nNumber of passes taken to sort the list:" << pass << endl;
    return 0;
    }
    

    template<typename RandomAccessIterator, typename Compare>       //Shell's sort
    void shell_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        for (auto d = (last - first) / 2; d != 0; d /= 2)
            //нужен цикл для first = a[0..d-1]
            for (auto i = first + d; i != last; ++i)
                for (auto j = i; j - first >= d && comp(*j, *(j - d)); j -= d)
                    std::swap(*j, *(j - d));
    }



}


int main() {                        //print of results
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
