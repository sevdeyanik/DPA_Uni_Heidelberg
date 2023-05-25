#include <iostream>
using namespace std;

int find_first_one(int arr[], int n) {
    int step = 1;
    int found_index = -1;

    while (step < n) {
        // Each processor checks its respective part of the array
        int local_index = -1;
        for (int i = found_index + step; i < n; i += step) {
            if (arr[i] == 1) {
                local_index = i;
                break;
            }
        }

        // Each processor performs an EREW exclusive write to update the minimum index
        if (local_index != -1) {
            found_index = (found_index == -1) ? local_index : min(found_index, local_index);
        }

        // Double the step value
        step *= 2;
    }

    return found_index;
}

int main() {
    // Initialize array of boolean elements
    int arr[7] = {0, 0, 0, 0, 1, 1, 1};
    int size = sizeof(arr);
    int index = find_first_one(arr, size);

    if (index != -1) {
        cout << "First occurrence of 1 found at index: " << index << endl;
    } else {
        cout << "No 1 found in the array." << endl;
    }

    return 0;
}
