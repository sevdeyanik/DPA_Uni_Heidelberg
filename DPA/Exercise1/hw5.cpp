#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;


double scalar_product(vector<double> a, vector<double> b)
{
    double product = 0;
    if (a.size() != b.size()) {
        cout << "Dot product cannot be calculated" << endl;
        return -1;
    }

    
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

    cout << scalar_product(veca, vecb) << endl;

    return 0;
}
