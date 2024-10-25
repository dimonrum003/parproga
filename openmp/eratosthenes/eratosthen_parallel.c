#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>

// Параллельная версия решета Эратосфена с OpenMP
void parallel_sieve(int N)
{
    bool *is_prime = (bool *) malloc((N + 1) * sizeof(bool));
    for (int i = 0; i <= N; i++)
    {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    int limit = sqrt(N);
    
    #pragma omp parallel for schedule(dynamic)
    for (int i = 2; i <= limit; i++)
    {
        if (is_prime[i])
        {
            for (int j = i * i; j <= N; j += i)
            {
                is_prime[j] = false;
            }
        }
    }

    printf("Простые числа до %d: ", N);
    for (int i = 2; i <= N; i++)
    {
        if (is_prime[i])
        {
            printf("%d ", i);
        }
    }
    printf("\n");
    free(is_prime);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Использование: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    double start_par = omp_get_wtime();
    parallel_sieve(N);
    double end_par = omp_get_wtime();
    printf("Время работы параллельной версии: %f секунд\n", end_par - start_par);

    return 0;
}