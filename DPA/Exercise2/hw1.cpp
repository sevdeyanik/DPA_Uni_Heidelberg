#include <iostream>
#include <vector>
#include <random>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {

    // MPI environment initialized
    MPI_Init(&argc, &argv);

    int rank, size;
    // The rank and size of the current process is retrieved
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the vector size and broadcast size
    const int vector_size = 100000;
    const int broadcast_size = vector_size * sizeof(int);

    // Create a random vector in the source process
    vector<int> vector_data(vector_size);
    if (rank == 0) {
        // Generate random numbers
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 100);
        for (int i = 0; i < vector_size; ++i) {
            vector_data[i] = dis(gen);
        }
    }

    // Broadcast the vector from the source process to all other processes
    double start_time = MPI_Wtime();
    MPI_Bcast(vector_data.data(), broadcast_size, MPI_BYTE, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    // Calculate the sum of the 3 least significant bits in each process
    int bits_sum = 0;
    for (int i = 0; i < vector_size; ++i) {
        // 0x7 is a bitmask for the 3 least significant bits
        bits_sum += vector_data[i] & 0x7;
    }

    // Print rank and bits_sum in each process
    cout << "Rank " << rank << ": Bits sum = " << bits_sum << endl;

    // Print the total time spent in the broadcast operation
    if (rank == 0) {
        double total_time = (end_time - start_time)* 1000.0;
        cout << "Total broadcast time: " << total_time << " ms" << endl;
    }
    // MPI environment finalized
    MPI_Finalize();
    return 0;
}
