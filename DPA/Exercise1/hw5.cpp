#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Function to calculate the scalar(dot) product of two vectors
double scalar_product(vector<double> a, vector<double> b)
{
    double product = 0;

    // Check if the vectors have the same length
    if (a.size() != b.size()) {
        cout << "Dot product cannot be calculated" << endl;
        return -1;
    }

    // Use OpenMP parallel for loop to parallelize the computation
    #pragma omp parallel for reduction(+:product)
    for (int i = 0; i < a.size(); i++) {
        product += a[i] * b[i];
    }

    return product;
}

int main() {
    vector<double> veca(2);
    vector<double> vecb(2);

    veca[0] = 1.5;
    veca[1] = 0.7;
    vecb[0] = 1.0;
    vecb[1] = 0.7;

    // Call scalar_product function to calculate the scalar product and print the result
    cout << scalar_product(veca, vecb) << endl;

    return 0;
}
