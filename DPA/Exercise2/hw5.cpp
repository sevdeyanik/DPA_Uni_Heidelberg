#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <omp.h>

std::vector<int> compute_array_B(const std::vector<bool>& A) {
    int n = A.size();
    int log_n = std::log2(n);
    int n_over_log_n = n / log_n;

    std::vector<int> B(n, 0);
    std::vector<int> count_values(n_over_log_n, 0);

    // Step 1: Compute counts in parallel
    #pragma omp parallel for schedule(static) num_threads(n_over_log_n)
    for (int i = 0; i < n_over_log_n; i++) {
        int count = 0;
        int start = i * log_n;
        int end = start + log_n;

        for (int j = start; j < end; j++) {
            if (A[j]) {
                count++;
            }
        }

        count_values[i] = count;
    }

    // Step 2: Compute prefix sum in parallel
    for (int h = 1; h <= log_n; h++) {
        int offset = std::pow(2, h - 1);
        #pragma omp parallel for schedule(static) num_threads(n_over_log_n) shared(count_values)
        for (int i = offset; i < n_over_log_n; i++) {
            count_values[i] += count_values[i - offset];
        }
    }

    // Step 3: Scatter non-zero elements to B
    #pragma omp parallel for schedule(static) num_threads(n_over_log_n)
    for (int i = 0; i < n_over_log_n; i++) {
        int start = i * log_n;
        int count = count_values[i];
        int end = start + log_n;
        int idx = 0;

        for (int j = start; j < end; j++) {
            if (A[j]) {
                B[count - 1 + idx] = j + 1;
                idx++;
            }
        }
    }

    // Step 4: Sort array B in ascending order
    std::sort(B.begin(), B.end());

    // Step 5: Delete zeros from B
    B.erase(std::remove(B.begin(), B.end(), 0), B.end());

    return B;
}

int main() {
    std::vector<bool> A = {false, true, false, true, true, false, false, true, true, true};
    std::vector<int> B = compute_array_B(A);

    std::cout << "Array B: ";
    for (int num : B) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
