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

    // Compute xi using the linear recurrence
    x[0] = a[0];
    for (int i = 1; i < n; i++) {
        x[i] = x[i - 1] * a[i] + b[i];
    }

    // Print the computed terms xi
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = " << x[i] << endl;
    }

    MPI_Finalize();
    return 0;
}
