#include <iostream>
using namespace std;

int binarySearch(int[], int, int, int)

int main()
{
    int num[10] = {10, 22, 36, 55,94,119}
    int search_num, loc =- 1;

    cout<<"Enter the numer that you want to search: ";
    cin >> search_num;

    loc = binarySearch(num, 0, 6, search_num);

    if(loc != -1)
    {
        cout << search_num << "Found in the array at the location:" << loc;
    }
    else
    {
        cout<<"Element not found";
    }
    return 0;
}

int binarySearch(int a[], int first, int last, int search_num)
{
    int middle;
    if (last >= first)
    {
        middle = (first+last)/2;

        if (a[middle] == search_num)
        {
            return middle++;
        }

        else  if(a[middle] < search_num)
        {
            return binarySearch(a, middle++, last, search_num);
        }
        else
        {
            return binarySearch(a, first, middle--, search_num);
        }
    }
    return -1;
}
