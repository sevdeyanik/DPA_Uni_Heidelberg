#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

// Helper function to print the ranks of the list
void print_ranks(const vector<int>& ranks) {
    cout << "Ranks: ";
    for (int rank : ranks) {
        cout << rank << " ";
    }
    cout << endl;
}

// Sequential List Ranking algorithm
void list_ranking_sequential(vector<int>& list, vector<int>& ranks) {
    int size = list.size();

    // Create a copy of the list and sort it
    vector<int> sorted_list = list;
    sort(sorted_list.begin(), sorted_list.end());

    // Compute ranks
    for (int i = 0; i < size; i++) {
        ranks[i] = lower_bound(sorted_list.begin(), sorted_list.end(), list[i]) - sorted_list.begin();
    }
}

int main() {
    const int size = 1000;
    const int max_value = 2000;

    // Create a vector to store the array
    vector<int> arr(size);

    // Generate unique random values
    vector<int> values(max_value);
    for (int i = 0; i < max_value; i++) {
        values[i] = i + 1;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(values.begin(), values.end(), g);

    // Fill the array with the shuffled values
    for (int i = 0; i < size; i++) {
        arr[i] = values[i];
    }

    // Print the array
    cout << "Array: ";
    for (int i : arr) {
        cout << i << " ";
    }
    cout << endl;
    vector<int> list = arr;
    // int size = list.size();

    vector<int> ranks(size);

    // Start measuring the running time
    auto start_time = chrono::steady_clock::now();

    list_ranking_sequential(list, ranks);

    // Stop measuring the running time
    auto end_time = chrono::steady_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

    print_ranks(ranks);

    cout << "Sequential algorithm running time: " << elapsed_time << " microseconds" << endl;

    return 0;
}
