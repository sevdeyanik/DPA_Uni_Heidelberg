#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

int main() {
    int INTERVAL = 10000;
    int circle_points = 0;
    int square_points = 0;
    int interval, i;
    float x, y, d, pi;

    i = 1000000;

    // Set the number of threads
    omp_set_num_threads(4);

    double start_time = omp_get_wtime();

    #pragma omp parallel private(x, y, d) reduction(+:circle_points, square_points)
    {
        unsigned int seed = time(NULL) + omp_get_thread_num();
        int local_circle_points = 0;
        int local_square_points = 0;

        #pragma omp for nowait
        for (interval = 0; interval < i; interval++) {
            x = double(rand_r(&seed) % (INTERVAL + 1)) / INTERVAL;
            y = double(rand_r(&seed) % (INTERVAL + 1)) / INTERVAL;
            d = x * x + y * y;
            if (d <= 1)
                local_circle_points++;
            local_square_points++;
        }

        circle_points += local_circle_points;
        square_points += local_square_points;
    }

    pi = double(4 * circle_points) / square_points;
    double end_time = omp_get_wtime();
    double running_time = (end_time - start_time) * 1000.0;

    cout << "\nFinal Estimation of Pi = " << pi << endl;
    cout << "Running time: " << running_time << " ms" << endl;

    return 0;
}
