#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>

using namespace std;

int find_first_one(vector<bool>& A) {
    int n = A.size();
    int num_procs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local_size = n / num_procs;
    int local_start = rank * local_size;
    int local_end = local_start + local_size - 1;

    int local_first_one = -1;

    // Search for the first one in the local portion of the array
    for (int i = local_start; i <= local_end; i++) {
        if (A[i]) {
            local_first_one = i;
            break;
        }
    }

    int global_first_one;
    MPI_Reduce(&local_first_one, &global_first_one, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        if (global_first_one != -1) {
            cout << "First one found at index: " << global_first_one << endl;
        } else {
            cout << "No one found!" << endl;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    vector<bool> A;
    A.push_back(0);
    A.push_back(0);
    A.push_back(0);
    A.push_back(1);
    A.push_back(0);
    A.push_back(0);
    A.push_back(1);
    A.push_back(1);

    find_first_one(A);

    MPI_Finalize();
    return 0;
}
