#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double startTime;

static inline void swap(int *v, int i, int j)
{
  int t = v[i];
  v[i] = v[j];
  v[j] = t;
}

void quicksort(int *v, int s, int n)
{
  int x, p, i;

  if (n <= 1)
    return;

  x = v[s + n / 2];
  swap(v, s, s + n / 2);

  p = s;
  for (i = s + 1; i < s + n; i++)
    if (v[i] < x)
    {
      p++;
      swap(v, i, p);
    }

  swap(v, s, p);

  quicksort(v, s, p - s);
  quicksort(v, p + 1, s + n - p - 1);
}

int *merge(int *v1, int n1, int *v2, int n2)
{
  int *result = (int *)malloc((n1 + n2) * sizeof(int));
  int i = 0;
  int j = 0;
  int k = 0;

  while (i < n1 && j < n2)
  {
    if (v1[i] < v2[j])
    {
      result[k] = v1[i];
      i++;
    }
    else
    {
      result[k] = v2[j];
      j++;
    }
    k++;
  }

  while (i < n1)
  {
    result[k] = v1[i];
    i++;
    k++;
  }

  while (j < n2)
  {
    result[k] = v2[j];
    j++;
    k++;
  }

  return result;
}

int main(int argc, char **argv)
{
  int n = 10;
  int data[] = {4, 2, 8, 1, 5, 7, 3, 6, 9, 0};
  int c, s;
  int *chunk;
  int o;
  int *other;
  int step;
  int p, id;
  MPI_Status status;
  double elapsed_time;
  int i;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  c = (n % p != 0) ? n / p + 1 : n / p;

  chunk = (int *)malloc(c * sizeof(int));
  MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);

  s = (n >= c * (id + 1)) ? c : n - c * id;
  quicksort(chunk, 0, s);

  for (step = 1; step < p; step = 2 * step)
  {
    if (id % (2 * step) != 0)
    {
      MPI_Send(chunk, s, MPI_INT, id - step, 0, MPI_COMM_WORLD);
      break;
    }

    if (id + step < p)
    {
      o = (n >= c * (id + 2 * step)) ? c * step : n - c * (id + step);
      other = (int *)malloc(o * sizeof(int));
      MPI_Recv(other, o, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);

      int *merged = merge(chunk, s, other, o);
      free(chunk);
      free(other);
      chunk = merged;
      s = s + o;

      quicksort(chunk, 0, s); // Sort the merged chunk again
    }
  }

  elapsed_time += MPI_Wtime();

  if (id == 0)
  {
    for (i = 0; i < s; i++)
      printf("%d ", chunk[i]);
    printf("\n");
  }

  printf("Quicksort %d ints on %d procs: %f secs\n", n, p, elapsed_time);

  MPI_Finalize();
  return 0;
}
