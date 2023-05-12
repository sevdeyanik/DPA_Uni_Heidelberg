#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

vector<double> matrix_vector_product(vector<vector<double>>& matrix, vector<double>& vec)
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    if (cols != vec.size()) {
        cout << "Matrix-vector product cannot be calculated" << endl;
        return {};
    }

    vector<double> result(rows, 0.0);

    #pragma omp parallel
    {
        // Set the desired number of threads
        omp_set_num_threads(4);

        #pragma omp for
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[i] += matrix[i][j] * vec[j];
            }
        }
    }

    return result;
}

int main() {
    vector<vector<double>> matrix = {{1.0, 2.0}, {3.0, 4.0}};
    vector<double> vec = {1.0, 2.0};

    vector<double> result = matrix_vector_product(matrix, vec);

    cout << "Result: ";
    for (double val : result) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
