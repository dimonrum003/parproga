#include <stdio.h>
#include <omp.h>

#define NUM_ITER 65    // Число итераций
#define NUM_THREADS 4  // Число потоков

void test_static(int chunk_size)
{
    printf("\nStatic schedule with chunk size: %d\n", chunk_size);
    
    #pragma omp parallel for num_threads(NUM_THREADS) schedule(static, chunk_size)
    for (int i = 0; i < NUM_ITER; i++) 
    {
        printf("Iteration %d executed by thread %d\n", i, omp_get_thread_num());
    }
}

void test_dynamic(int chunk_size)
{
    printf("\nDynamic schedule with chunk size: %d\n", chunk_size);
    
    #pragma omp parallel for num_threads(NUM_THREADS) schedule(dynamic, chunk_size)
    for (int i = 0; i < NUM_ITER; i++)
    {
        printf("Iteration %d executed by thread %d\n", i, omp_get_thread_num());
    }
}

void test_guided(int chunk_size)
{
    printf("\nGuided schedule with chunk size: %d\n", chunk_size);
    
    #pragma omp parallel for num_threads(NUM_THREADS) schedule(guided, chunk_size)
    for (int i = 0; i < NUM_ITER; i++)
    {
        printf("Iteration %d executed by thread %d\n", i, omp_get_thread_num());
    }
}

int main()
{
    // Поведение по умолчанию (без директивы schedule)
    printf("Default schedule (no schedule directive):\n");
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < NUM_ITER; i++)
    {
        printf("Iteration %d executed by thread %d\n", i, omp_get_thread_num());
    }

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
