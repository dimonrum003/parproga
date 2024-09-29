#include <stdio.h>
#include <omp.h>

int main()
{
    int shared_var = 0; // Общая переменная

    #pragma omp parallel for ordered shared(shared_var)
    for (int i = 0; i < omp_get_num_threads(); i++)
    {
        #pragma omp ordered
        {
            shared_var += 1;
            printf("Thread %d accessed the variable. Current value: %d\n", i, shared_var);
        }
    }

    return 0;
}