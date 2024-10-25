#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>

// Последовательная версия решета Эратосфена
void sequential_sieve(int N)
{
    bool *is_prime = (bool *) malloc((N + 1) * sizeof(bool));
    for (int i = 0; i <= N; i++)
    {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i <= sqrt(N); i++)
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

    double start = omp_get_wtime();
    sequential_sieve(N);
    double end = omp_get_wtime();
    printf("Время работы последовательной версии: %f секунд\n", end - start);

    return 0;
}