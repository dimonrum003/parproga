#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>

// Последовательная версия решета Эратосфена
void sequential_sieve(int N, bool *is_prime_seq)
{
    for (int i = 0; i <= N; i++)
    {
        is_prime_seq[i] = true;
    }
    is_prime_seq[0] = is_prime_seq[1] = false;

    for (int i = 2; i <= sqrt(N); i++)
    {
        if (is_prime_seq[i])
        {
            for (int j = i * i; j <= N; j += i)
            {
                is_prime_seq[j] = false;
            }
        }
    }
}

// Параллельная версия решета Эратосфена
void parallel_sieve(int N, bool *is_prime_par)
{
    for (int i = 0; i <= N; i++)
    {
        is_prime_par[i] = true;
    }
    is_prime_par[0] = is_prime_par[1] = false;

    int limit = sqrt(N);
    
    #pragma omp parallel for schedule(dynamic)
    for (int i = 2; i <= limit; i++)
    {
        if (is_prime_par[i])
        {
            for (int j = i * i; j <= N; j += i)
            {
                is_prime_par[j] = false;
            }
        }
    }
}

// Функция для сравнения результатов последовательной и параллельной версий
bool compare_results(int N, bool *is_prime_seq, bool *is_prime_par)
{
    for (int i = 0; i <= N; i++)
    {
        if (is_prime_seq[i] != is_prime_par[i])
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Использование: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    
    // Выделение памяти для результатов
    bool *is_prime_seq = (bool *) malloc((N + 1) * sizeof(bool));
    bool *is_prime_par = (bool *) malloc((N + 1) * sizeof(bool));

    // Последовательная версия
    double start_seq = omp_get_wtime();
    sequential_sieve(N, is_prime_seq);
    double end_seq = omp_get_wtime();
    
    // Параллельная версия
    double start_par = omp_get_wtime();
    parallel_sieve(N, is_prime_par);
    double end_par = omp_get_wtime();
    
    // Вывод простых чисел
    printf("Простые числа до %d: ", N);
    for (int i = 2; i <= N; i++)
    {
        if (is_prime_seq[i])
        {
            printf("%d ", i);
        }
    }
    printf("\n");

    // Вывод времен работы
    printf("Время работы последовательной версии: %f секунд\n", end_seq - start_seq);
    printf("Время работы параллельной версии: %f секунд\n", end_par - start_par);

    // Сравнение результатов
    if (compare_results(N, is_prime_seq, is_prime_par))
    {
        printf("Результаты совпадают\n");
    } else {
        printf("Результаты не совпадают\n");
    }

    // Освобождение памяти
    free(is_prime_seq);
    free(is_prime_par);

    return 0;
}