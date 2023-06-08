#include<iostream>
#include<algorithm>

using namespace std;

void mergeArrays(int A[], int B[], int n, int C[])
{
    int i = 0, j = 0, k = 0;
      // traverse the A and insert its element in C
      while(i < n){
      C[k++] = A[i++];
    }
       
      // now traverse B and insert in C
      while(j < n){
      C[k++] = B[j++];
    }
       
      // sort the whole array C
      sort(C, C + (2 * n));
}


int main() {
    int A[] = { 1, 5, 15, 20, 35};
    int B[] = { 7, 14, 23, 50, 70};
    int n = sizeof(A)/sizeof(A[0]);
    int C[2 * n];

    mergeArrays(A, B, n, C);

    int start = 0;
    int end = 2 * n - 1;
    int ai = 5;
    int j = -1;

    while (start <= end) {
        int middle_index = (start + end) / 2;

        if (ai == C[middle_index]) {
            j = middle_index + 1;
            break;
        }
        if (ai < C[middle_index]) {
            end = middle_index - 1;
        }
        else {
            start = middle_index + 1;

        }

    }
    cout << j << endl;
    
    return 0;
    
}
