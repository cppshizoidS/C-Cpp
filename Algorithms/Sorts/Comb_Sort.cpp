#include <iostream>
using namespace std;
void combsort(int a[], int n) {
   int i, j, gap, swapped = 1;
   double temp;
   gap = n;
   while (gap > 1 || swapped == 1) {
      gap = gap * 10 / 1.3;
      if (gap == 9 || gap == 10) {
         gap = 11;
      }
      if (gap < 1) {
         gap = 1;
      }
      swapped = 0;
      for (i = 0, j = gap; j < n; i++, j++) {
         if (a[i] > a[j]) {
            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            swapped = 1;
         }
      }
   }
}
int main () {
   int n, i;
   int arr[] = { 5, 3, 4, 2, 1 };
   n=5;
   combsort(arr, n);
   for(i = 0;i < n;i++) {
      cout<<arr[i]<<"\t";
   }
   return 0;
}
