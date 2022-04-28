#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *first, int *second)
{
    int temp = *first;
    *first = *second;
    *second = temp;
}

void bubbleSort(int *arr, int size)
{
    if (size == 1)
    {
        return;
    }
    bool swapped = false;
    for (int i = 0; i < size - 1; ++i)
    {
        if (arr[i] > arr[i + 1])
        {
            swap(arr + i, arr + i + 1);
            swapped = true;
        }
    }
    if (swapped)
    {
        bubbleSort(arr, size - 1);
    }
}

/*test*/
void test()
{
    const int size = 10;
    int *arr = (int *)calloc(size, sizeof(int));

    /* generate size random numbers from 0 to 100 */
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % 100;
    }
    bubbleSort(arr, size);
    for (int i = 0; i < size - 1; ++i)
    {
        assert(arr[i] <= arr[i + 1]);
    }
    free(arr);
}

int main()
{
    /*random number generator */
    srand(time(NULL));
    test();
    return 0;
}
