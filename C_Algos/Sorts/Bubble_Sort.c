#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void display(const int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap(int *first, int *second)
{
    int temp = *first;
    *first = *second;
    *second = temp;
}

void bubbleSort(int *arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {                         
        bool swapped = false; 
        for (int j = 0; j < size - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            { 
                swap(&arr[j], &arr[j + 1]);
                swapped = true; /* set flag */
            }
        }
        if (!swapped)
        {
            break;
        }
    }
}


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

int main(int argc, const char *argv[])
{
    srand(time(NULL));
    test();
    return 0;
}
