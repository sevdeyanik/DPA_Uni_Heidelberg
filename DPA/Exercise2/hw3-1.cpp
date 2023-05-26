#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Number of terms to compute
    const int n = 10;
    int a[n], b[n], x[n];

    // Initialize sequences a, b, and x
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
        b[i] = 1;
        x[i] = 0;
    }

    // Compute prefix-sums of sequence a
    int prefix_sum_a[n];
    MPI_Scan(a, prefix_sum_a, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Compute the exclusive prefix-sums on the last process
    if (rank == size - 1) {
        int prefix_sum_exclusive[n];
        prefix_sum_exclusive[0] = 0;
        for (int i = 1; i < n; i++) {
            prefix_sum_exclusive[i] = prefix_sum_a[i - 1];
        }

        // Compute xi using the linear recurrence
        x[0] = a[0];
        for (int i = 1; i < n; i++) {
            x[i] = prefix_sum_exclusive[i] * x[i - 1] + b[i];
        }
    }

    // Gather the computed terms xi to process 0
    int* all_x = nullptr;
    if (rank == 0) {
        all_x = new int[n * size];
    }
    MPI_Gather(x, n, MPI_INT, all_x, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the computed terms xi in process 0
    if (rank == 0) {
        for (int i = 0; i < n * size; i++) {
            cout << "x[" << i << "] = " << all_x[i] << endl;
        }
        delete[] all_x;
    }

    MPI_Finalize();
    return 0;
}
