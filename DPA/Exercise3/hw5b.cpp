#include <iostream>
#include <algorithm>
#include <mpi.h>

using namespace std;

void mergeArraysParallel(int* A, int* B, int n, int* C)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunkSize = n / size;
    int* localC = new int[2 * chunkSize];
    int* localA = new int[chunkSize];
    int* localB = new int[chunkSize];

    // Scatter arrays A and B
    MPI_Scatter(A, chunkSize, MPI_INT, localA, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, chunkSize, MPI_INT, localB, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Merge localA and localB into localC
    merge(localA, localA + chunkSize, localB, localB + chunkSize, localC);

    // Gather the merged arrays from all processes
    MPI_Gather(localC, 2 * chunkSize, MPI_INT, C, 2 * chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    delete[] localC;
    delete[] localA;
    delete[] localB;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int A[] = { 1, 5, 15, 20};
    int B[] = { 7, 14, 23, 50};
    int n = sizeof(A) / sizeof(A[0]);
    int C[2 * n];

    mergeArraysParallel(A, B, n, C);

    if (rank == 0) {
        sort(C, C + (2 * n));
        cout << "Merged array C: ";
        for (int i = 0; i < 2 * n; i++) {
            cout << C[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
