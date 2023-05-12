#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = rank; 
    int parent;

    double start_normal, end_normal, total_normal;
    double start_reverse, end_reverse, total_reverse;

    // Normal order binomial tree broadcast
    start_normal = MPI_Wtime();

    for (int i = 1; i < size; i *= 2) {
        if (rank % (2 * i) == 0) {
            if (rank + i < size) {
                MPI_Recv(&data, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } else {
            parent = rank - i;
            MPI_Send(&data, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
            break;
        }
    }

    end_normal = MPI_Wtime();
    total_normal = (end_normal - start_normal) * 1000.0;

    // Reverse order binomial tree broadcast
    start_reverse = MPI_Wtime();

    for (int i = size / 2; i > 0; i /= 2) {
        if (rank >= i) {
            parent = rank - i;
            MPI_Send(&data, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
            break;
        } else if (rank < i && rank + i < size) {
            MPI_Recv(&data, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    end_reverse = MPI_Wtime();
    total_reverse = (end_reverse - start_reverse) * 1000.0;

    std::cout << "Process " << rank << " received data in normal order: " << data << std::endl;
    std::cout << "Total time in normal order: " << total_normal << " seconds" << std::endl;

    std::cout << "Process " << rank << " received data in reverse order: " << data << std::endl;
    std::cout << "Total time in reverse order: " << total_reverse << " seconds" << std::endl;

    MPI_Finalize();
    return 0;
}
