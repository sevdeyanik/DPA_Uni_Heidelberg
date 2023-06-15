#include <iostream>
#include <mpi.h>

using namespace std;

#define TAG 0

int main(int argc, char** argv) {
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Generate a unique message for each process
    string message = "Message from process " + to_string(rank);

    // Asynchronous send messages to all other processes
    for (int dest = 0; dest < size; dest++) {
        if (dest != rank) {
            MPI_Request request;
            MPI_Isend(message.c_str(), message.size() + 1, MPI_CHAR, dest, TAG, MPI_COMM_WORLD, &request);
        }
    }

    // Receive messages from other processes
    for (int src = 0; src < size; src++) {
        if (src != rank) {
            char recv_message[100];
            MPI_Recv(recv_message, 100, MPI_CHAR, src, TAG, MPI_COMM_WORLD, &status);
            cout << "Process " << rank << " received message: " << recv_message << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
