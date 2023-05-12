#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

vector<double> matrix_vector_product(vector<vector<double>>& matrix, vector<double>& vec)
{
    // Get the dimensions of the matrix and vector
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Check if the dimensions match for matrix-vector multiplication
    if (cols != vec.size()) {
        cout << "Matrix-vector product cannot be calculated" << endl;
        return {};
    }

    // Initialize a result vector with the same number of rows as the matrix
    vector<double> result(rows, 0.0);

    // Parallelize the calculation of the matrix-vector product using OpenMP
    #pragma omp parallel
    {
        // Set the desired number of threads
        omp_set_num_threads(4);

        // Distribute the iterations of the outer loop across the threads
        #pragma omp for
        for (int i = 0; i < rows; i++) {
            // Calculate the dot product of the i-th row of the matrix and the vector
            for (int j = 0; j < cols; j++) {
                result[i] += matrix[i][j] * vec[j];
            }
        }
    }

    // Return the result vector
    return result;
}

int main() {
    // Define a matrix and a vector for the matrix-vector product
    vector<vector<double>> matrix = {{1.0, 2.0}, {3.0, 4.0}};
    vector<double> vec = {1.0, 2.0};

    // Calculate the matrix-vector product using the defined function
    vector<double> result = matrix_vector_product(matrix, vec);

    // Print the result vector
    cout << "Result: ";
    for (double val : result) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
