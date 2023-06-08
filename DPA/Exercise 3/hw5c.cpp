#include <iostream>
#include <algorithm>
#include <mpi.h>
#include <cmath>

using namespace std;

void mergeArraysParallel(int* A, int* B, int n, int* C)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine the chunk size for each process
    int chunkSize = n / size;

    // Scatter arrays A and B
    int* localA = new int[chunkSize];
    int* localB = new int[chunkSize];
    MPI_Scatter(A, chunkSize, MPI_INT, localA, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, chunkSize, MPI_INT, localB, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Merge localA and localB into a local merged array
    int* localMerged = new int[2 * chunkSize];
    merge(localA, localA + chunkSize, localB, localB + chunkSize, localMerged);

    // Gather the local merged arrays onto process 0
    MPI_Gather(localMerged, 2 * chunkSize, MPI_INT, C, 2 * chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    delete[] localA;
    delete[] localB;
    delete[] localMerged;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int A[] = {1, 5, 15, 20};
    int B[] = {7, 14, 23, 50};
    int n = sizeof(A) / sizeof(A[0]);
    int p;

    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (pow(2, static_cast<int>(log2(p))) != p) {
        if (rank == 0) {
            cout << "The number of processes must be a power of 2." << endl;
        }
        MPI_Finalize();
        return 0;
    }

    int* C = new int[2 * n];

    mergeArraysParallel(A, B, n, C);

    if (rank == 0) {
        sort(C, C + (2 * n));
        cout << "Merged array C: ";
        for (int i = 0; i < 2 * n; i++) {
            cout << C[i] << " ";
        }
        cout << endl;
    }

    delete[] C;

    MPI_Finalize();
    return 0;
}
