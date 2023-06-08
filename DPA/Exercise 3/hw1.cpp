#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

// Function to swap two numbers
void swap(vector<int>& arr, int i, int j) {
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

// Function that performs the Quick Sort
// for an array arr[] starting from the
// index start and ending at index end
void quicksort(vector<int>& arr, int start, int end) {
    int pivot, index;

    // Base Case
    if (end <= start)
        return;

    // Pick pivot and swap with first
    // element Pivot is middle element
    pivot = arr[start + (end - start) / 2];
    swap(arr, start, start + (end - start) / 2);

    // Partitioning Steps
    index = start;

    // Iterate over the range [start, end]
    for (int i = start + 1; i <= end; i++) {

        // Swap if the element is less
        // than the pivot element
        if (arr[i] < pivot) {
            index++;
            swap(arr, i, index);
        }
    }

    // Swap the pivot into place
    swap(arr, start, index);

    // Recursive Call for sorting
    // of quick sort function
    quicksort(arr, start, index - 1);
    quicksort(arr, index + 1, end);
}

// Function that merges the two arrays
vector<int> merge(vector<int>& arr1, vector<int>& arr2) {
    vector<int> result;
    int i = 0;
    int j = 0;

    while (i < arr1.size() && j < arr2.size()) {
        if (arr1[i] < arr2[j]) {
            result.push_back(arr1[i]);
            i++;
        } else {
            result.push_back(arr2[j]);
            j++;
        }
    }

    while (i < arr1.size()) {
        result.push_back(arr1[i]);
        i++;
    }

    while (j < arr2.size()) {
        result.push_back(arr2[j]);
        j++;
    }

    return result;
}

// Driver Code
int main(int argc, char* argv[]) {
    int number_of_elements = 4;
    vector<int> data;
    data.resize(number_of_elements);
    data[0] = 9;
    data[1] = 5;
    data[2] = 7;
    data[3] = 2;



    int chunk_size, own_chunk_size;
    vector<int> chunk;
    double time_taken;
    MPI_Status status;

    int number_of_process, rank_of_process;
    int rc = MPI_Init(&argc, &argv);

    if (rc != MPI_SUCCESS) {
        cout << "Error in creating MPI program. Terminating..." << endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &number_of_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_of_process);

    if (rank_of_process == 0) {
        number_of_elements = data.size();

        // Computing chunk size
        chunk_size = (number_of_elements % number_of_process == 0)
                         ? (number_of_elements / number_of_process)
                         : (number_of_elements / number_of_process - 1);

        // Padding data with zeros
        data.resize(number_of_process * chunk_size, 0);

        // Printing the array read from input
        cout << "Elements in the array: ";
        for (int i = 0; i < number_of_elements; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    // Blocks all process until reach this point
    MPI_Barrier(MPI_COMM_WORLD);

    // Starts Timer
    time_taken -= MPI_Wtime();

    // Broadcast the Size to all the
    // process from root process
    MPI_Bcast(&number_of_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Computing chunk size
    chunk_size = (number_of_elements % number_of_process == 0)
                     ? (number_of_elements / number_of_process)
                     : number_of_elements / (number_of_process - 1);

    // Calculating total size of chunk
    // according to bits
    chunk.resize(chunk_size);

    // Scatter the chunk size data to all process
    MPI_Scatter(data.data(), chunk_size, MPI_INT, chunk.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute size of own chunk and
    // then sort them using quick sort
    own_chunk_size = (number_of_elements >= chunk_size * (rank_of_process + 1))
                         ? chunk_size
                         : (number_of_elements - chunk_size * rank_of_process);

    // Sorting array with quick sort for every
    // chunk as called by process
    quicksort(chunk, 0, own_chunk_size - 1);

    for (int step = 1; step < number_of_process; step = 2 * step) {
        if (rank_of_process % (2 * step) != 0) {
            MPI_Send(chunk.data(), own_chunk_size, MPI_INT, rank_of_process - step, 0, MPI_COMM_WORLD);
            break;
        }

        if (rank_of_process + step < number_of_process) {
            int received_chunk_size = (number_of_elements >= chunk_size * (rank_of_process + 2 * step))
                                          ? (chunk_size * step)
                                          : (number_of_elements - chunk_size * (rank_of_process + step));
            vector<int> chunk_received(received_chunk_size);
            MPI_Recv(chunk_received.data(), received_chunk_size, MPI_INT, rank_of_process + step, 0, MPI_COMM_WORLD, &status);

            data = merge(chunk, chunk_received);

            chunk = data;
            own_chunk_size = chunk.size();
        }
    }

    // Stop the timer
    time_taken += MPI_Wtime();

    if (rank_of_process == 0) {
        // Printing sorted array
        cout << "Sorted array is: ";
        for (int i = 0; i < number_of_elements; i++) {
            cout << chunk[i] << " ";
        }
        cout << endl;

        cout << "\nQuicksort " << number_of_elements << " ints on " << number_of_process << " procs: " << time_taken << " secs" << endl;
    }

    MPI_Finalize();
    return 0;
}
