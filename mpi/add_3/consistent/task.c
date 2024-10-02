#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// PS: компилировать с флагами "-lgmp -lm"

void compute_e(int precision)
{
    // Установка точности переменных mpf_t
    mpf_set_default_prec(precision * 3.33); // Множитель 3.33 для достаточной точности

    mpz_t factorial;
    mpf_t e_partial, term, sum;
    mpz_init(factorial);
    mpf_init(e_partial);
    mpf_init(term);
    mpf_init(sum);

    mpz_set_ui(factorial, 1);
    mpf_set_ui(e_partial, 1);
    mpf_set_ui(sum, 1); // Начальное значение суммы равно 1 (e^0)

    for (int i = 1; i <= 1000; ++i)
    {
        // Вычисление i-го члена ряда Тейлора: 1 / (i!)
        mpz_mul_ui(factorial, factorial, i);
        mpf_set_z(term, factorial);
        mpf_ui_div(term, 1, term);

        // Суммирование членов ряда
        mpf_add(sum, sum, term);

        // Проверка точности
        if (i > 1 && mpf_cmp(term, e_partial) == 0)
            break;

        mpf_set(e_partial, term);
    }

    // Вывод результата с заданной точностью
    printf("e calculated with %d decimal places:\n", precision);
    mpf_out_str(stdout, 10, precision + 1, sum);
    printf("\n");

    mpz_clear(factorial);
    mpf_clear(e_partial);
    mpf_clear(term);
    mpf_clear(sum);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s precision\n", argv[0]);
        return 1;
    }

    int precision = atoi(argv[1]);

    if (precision <= 0)
    {
        printf("Precision must be a positive integer.\n");
        return 1;
    }

    compute_e(precision);

    return 0;
}