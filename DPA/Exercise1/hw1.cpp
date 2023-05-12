#include <iostream>
#include <chrono>
using namespace std;

int main() {
  // Initialize variables
  int INTERVAL = 10000;
  int circle_points = 0;
  int square_points = 0;
  int interval, i;
  float x, y, d, pi;

  i = 1000000;

  // Record the start time
  auto start_time = chrono::high_resolution_clock::now();

  for (interval = 0; interval < i; interval++){
    // Generate random points
    x = double(rand() % (INTERVAL + 1)) / INTERVAL;
    y = double(rand() % (INTERVAL + 1)) / INTERVAL;
    // Calculate if the point is in the unit circle
    d = x*x + y*y;
    if (d <= 1)
      circle_points++;
    square_points++;
    // Calculate Pi
    pi = double(4 * circle_points) / square_points;
  }

  // Record the end time
  auto end_time = chrono::high_resolution_clock::now();
  // Calculate the running time in milliseconds
  auto running_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
  
  // Print out the results
  cout << "\nFinal Estimation of Pi = " << pi << endl;
  cout << "Running time: " << running_time << " ms" << endl;

  return 0;
}