#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

int main(int argc, char **argv) {
    int INTERVAL = 10000;
    int circle_points = 0;
    int square_points = 0;
    int interval = 1000000;
    float x, y, d, pi;

    int rank, size;
    // MPI environment initialized
    MPI_Init(&argc, &argv);
    // The rank and size of the current process is retrieved
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // The number of points to be generated are divided equally among the processes
    int sub_interval = interval / size;
    int sub_circle_points = 0;
    int sub_square_points = 0;

    srand(time(NULL) + rank);

    // Start the timer
    double start_time = MPI_Wtime();

    // Generate random points and compute the number of points inside the circle for each process
    for (int i = 0; i < sub_interval; i++) {
        x = double(rand() % (INTERVAL + 1)) / INTERVAL;
        y = double(rand() % (INTERVAL + 1)) / INTERVAL;
        d = x * x + y * y;
        if (d <= 1)
            sub_circle_points++;
        sub_square_points++;
    }

    // Reduce the results from all processes and compute the final result
    MPI_Reduce(&sub_circle_points, &circle_points, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sub_square_points, &square_points, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    // Print the final result and the running time for the root process
    if (rank == 0) {
        pi = double(4 * circle_points) / square_points;
        double end_time = MPI_Wtime();
        cout << "\nFinal Estimation of Pi = " << pi << endl;
        double running_time = (end_time - start_time) * 1000.0;
        cout << "Running time: " << running_time << " ms" << endl;
    }
    // MPI environment finalized
    MPI_Finalize();
    return 0;
}