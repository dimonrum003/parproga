#include <omp.h>
#include <stdio.h>

void unsafe_function()
{
    int stack_variable = 42;  // Локальная переменная на стеке
    #pragma omp task untied
    {
        // Попытка использовать локальную переменную из другого потока
        printf("Value: %d\n", stack_variable);
    }
}

int main()
{
    #pragma omp parallel
    {
        #pragma omp single
        {
            unsafe_function();
        }
    }
    return 0;
}