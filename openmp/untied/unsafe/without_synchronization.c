#include <omp.h>
#include <stdio.h>

int shared_counter = 0;  // Глобальная разделяемая переменная

void increment_counter()
{
    #pragma omp task untied
    {
        for (int i = 0; i < 1000; i++)
        {
            shared_counter++;  // Несинхронизированный доступ
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
                increment_counter();
            }
        }
    }

    printf("Final Counter Value: %d\n", shared_counter);
    return 0;
}