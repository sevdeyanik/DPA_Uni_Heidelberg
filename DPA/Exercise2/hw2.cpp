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

    // Binomial tree broadcast
    double start_time = MPI_Wtime();

    int num_steps = 0;
    int power_of_two = 1;
    while (power_of_two < size) {
        if (rank < power_of_two) {
            int dest = rank + power_of_two;
            if (dest < size) {
                MPI_Send(vector_data.data(), broadcast_size, MPI_BYTE, dest, 0, MPI_COMM_WORLD);
            }
        }
        else if (rank < 2 * power_of_two) {
            int source = rank - power_of_two;
            MPI_Recv(vector_data.data(), broadcast_size, MPI_BYTE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        power_of_two *= 2;
        num_steps++;
    }

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
        double total_time = (end_time - start_time) * 1000.0;
        cout << "Total broadcast time: " << total_time << " ms" << endl;
        cout << "Number of steps: " << num_steps << endl;
    }

    // MPI environment finalized
    MPI_Finalize();
    return 0;
}
