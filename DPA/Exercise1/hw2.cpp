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

    i = 1000000;  // Number of iterations

    // Set the number of threads
    omp_set_num_threads(4);

    double start_time = omp_get_wtime();  // Record start time

    #pragma omp parallel private(x, y, d) reduction(+:circle_points, square_points)
    {
        // Use the thread ID as the seed for random number generation
        unsigned int seed = time(NULL) + omp_get_thread_num();
        int local_circle_points = 0;
        int local_square_points = 0;

        // Each thread runs a subset of the iterations
        #pragma omp for nowait
        for (interval = 0; interval < i; interval++) {
            x = double(rand_r(&seed) % (INTERVAL + 1)) / INTERVAL;  // Generate a random x-coordinate
            y = double(rand_r(&seed) % (INTERVAL + 1)) / INTERVAL;  // Generate a random y-coordinate
            d = x * x + y * y;  // Compute the distance from the origin
            if (d <= 1)  // Check if the point is inside the unit circle
                local_circle_points++;
            local_square_points++;
        }

        // Accumulate the counts for each thread
        circle_points += local_circle_points;
        square_points += local_square_points;
    }

    pi = double(4 * circle_points) / square_points;  // Estimate Pi from the accumulated counts
    double end_time = omp_get_wtime();  // Record end time
    double running_time = (end_time - start_time) * 1000.0;  // Compute running time in milliseconds

    cout << "\nFinal Estimation of Pi = " << pi << endl;
    cout << "Running time: " << running_time << " ms" << endl;

    return 0;
}
