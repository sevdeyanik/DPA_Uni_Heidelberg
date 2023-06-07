#include<iostream>
#include <omp.h>
using namespace std;

void rev(int arr[], int i) {
   int temp, st = 0;
   while (st < i) {
      temp = arr[st];
      arr[st] = arr[i];
      arr[i] = temp;
      st++;
      i--;
   }
}

int maxIndex(int arr[], int n) {
   int index, i;
   for (index = 0, i = 0; i < n; ++i) {
      if (arr[i] > arr[index]) {
         index = i;
      }
   }
   return index;
}

int pancakeSort(int arr[], int n) {
   #pragma omp parallel for shared(arr, n)
   for (int size = n; size > 1; size--) {
      int index = maxIndex(arr, size);
      if (index != size-1) {
         rev(arr, index);
         rev(arr, size-1);
      }
   }

   // Return a value as required by the function declaration
   return 0;
}

int main() {
   int arr[] = {54, 85, 52, 25, 98, 75, 25, 11, 68};
   int n = sizeof(arr)/sizeof(arr[0]);
   pancakeSort(arr, n);
   cout << "Sorted array: ";
   for (int i = 0; i < n; ++i)
      cout << arr[i] << " ";
   cout << endl;
   return 0;
}
