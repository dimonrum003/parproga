#include <stdio.h>
#include <omp.h>

#define NUM_ITER 65    // Число итераций
#define NUM_THREADS 4  // Число потоков

// Функция для вычисления факториала числа n
unsigned long long factorial(int n)
{
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

void test_static(int chunk_size)
{
    double start_time, end_time;
    start_time = omp_get_wtime();  // Начало измерения времени

    printf("\nStatic schedule with chunk size: %d\n", chunk_size);
    
    #pragma omp parallel for num_threads(NUM_THREADS) schedule(static, chunk_size)
    for (int i = 0; i < NUM_ITER; i++)
    {
        unsigned long long fact = factorial(i);
        printf("Iteration %d executed by thread %d, factorial(%d) = %llu\n", i, omp_get_thread_num(), i, fact);
    }

    end_time = omp_get_wtime();  // Конец измерения времени
    printf("Static schedule with chunk size %d completed in %f seconds\n", chunk_size, end_time - start_time);
}

void test_dynamic(int chunk_size)
{
    double start_time, end_time;
    start_time = omp_get_wtime();  // Начало измерения времени

    printf("\nDynamic schedule with chunk size: %d\n", chunk_size);
    
    #pragma omp parallel for num_threads(NUM_THREADS) schedule(dynamic, chunk_size)
    for (int i = 0; i < NUM_ITER; i++)
    {
        unsigned long long fact = factorial(i);
        printf("Iteration %d executed by thread %d, factorial(%d) = %llu\n", i, omp_get_thread_num(), i, fact);
    }

    end_time = omp_get_wtime();  // Конец измерения времени
    printf("Dynamic schedule with chunk size %d completed in %f seconds\n", chunk_size, end_time - start_time);
}

void test_guided(int chunk_size)
{
    double start_time, end_time;
    start_time = omp_get_wtime();  // Начало измерения времени

    printf("\nGuided schedule with chunk size: %d\n", chunk_size);
    
    #pragma omp parallel for num_threads(NUM_THREADS) schedule(guided, chunk_size)
    for (int i = 0; i < NUM_ITER; i++)
    {
        unsigned long long fact = factorial(i);
        printf("Iteration %d executed by thread %d, factorial(%d) = %llu\n", i, omp_get_thread_num(), i, fact);
    }

    end_time = omp_get_wtime();  // Конец измерения времени
    printf("Guided schedule with chunk size %d completed in %f seconds\n", chunk_size, end_time - start_time);
}

int main()
{
    double start_time, end_time;

    // Поведение по умолчанию (без директивы schedule)
    start_time = omp_get_wtime();
    printf("Default schedule (no schedule directive):\n");
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < NUM_ITER; i++){
        unsigned long long fact = factorial(i);
        printf("Iteration %d executed by thread %d, factorial(%d) = %llu\n", i, omp_get_thread_num(), i, fact);
    }
    end_time = omp_get_wtime();
    printf("Default schedule completed in %f seconds\n", end_time - start_time);

    // Static schedule с chunk size 1 и 4
    test_static(1);
    test_static(4);

    // Dynamic schedule с chunk size 1 и 4
    test_dynamic(1);
    test_dynamic(4);

    // Guided schedule с chunk size 1 и 4
    test_guided(1);
    test_guided(4);

    return 0;
}
