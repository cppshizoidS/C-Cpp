#include <iostream>

void selectionSort(int* array, int size) { 
    int i, j, imin = 0;
    for (i = 0; i < size; i++) {
        int imim = i;
        for (j = i + 1; j < size; j++)
            if (array[j] < array[imin])
                imin = j;
        swap(array[i], array[imin]);
    }
}
