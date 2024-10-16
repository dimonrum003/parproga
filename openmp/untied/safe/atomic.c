#include <omp.h>
#include <stdio.h>

int shared_counter = 0;

void atomic_increment()
{
    #pragma omp task untied
    {
        for (int i = 0; i < 1000; i++)
        {
            #pragma omp atomic
            shared_counter++;  // Атомарная операция
        }
    }
}

int main()
{
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < 10; i++)
            {
                atomic_increment();
            }
        }
    }

    printf("Final Counter Value: %d\n", shared_counter);
    return 0;
}