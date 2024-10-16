#include <omp.h>
#include <stdio.h>

void safe_function(int *shared_array, int i)
{
    #pragma omp task untied
    {
        // Выполняется чистая вычислительная задача
        shared_array[i] = i * i;
    }
}

int main()
{
    int shared_array[10];

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < 10; i++)
            {
                safe_function(shared_array, i);
            }
        }
    }

    // Печать результатов
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", shared_array[i]);
    }
    printf("\n");
    
    return 0;
}