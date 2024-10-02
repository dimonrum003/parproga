#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size;
    int N = 0;
    double sum = 0.0;
    double local_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc > 1)
    {
        N = atoi(argv[1]);
    }

    // Распределение работы между процессами
    int start = rank * (N / size) + 1;
    int end = (rank + 1) * (N / size);
    if (rank == size - 1)
    {
        end = N;
    }

    // Вычисление локальной суммы
    for (int i = start; i <= end; i++)
    {
        local_sum += 1.0 / i;
    }

    // Сбор локальных сумм и вычисление общей суммы на процессе с рангом 0
    MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Вывод результата на процессе с рангом 0
    if (rank == 0)
    {
        printf("Сумма 1/n, где n меняется от 1 до %d равна: %.6f\n", N, sum);
    }

    MPI_Finalize();

    return 0;
}