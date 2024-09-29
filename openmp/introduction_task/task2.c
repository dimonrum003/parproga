#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]); // Получаем значение N из аргументов командной строки
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= N; i++)
    {
        sum += 1.0 / i; // Каждый поток считает свою часть суммы и аккумулирует через редукцию
    }

    printf("The sum 1/n where n is changing from 1 to %d equals: %f\n", N, sum);
    return 0;
}