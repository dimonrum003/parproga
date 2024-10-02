#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpi.h>
#include <time.h>

void compute_partial_sum(int start, int end, int precision, mpf_t result)
{
    mpz_t factorial;
    mpf_t term;
    mpz_init(factorial);
    mpf_init(term);

    mpz_set_ui(factorial, 1);
    mpf_set_ui(result, 0);

    for (int i = start; i <= end; ++i)
    {
        // Вычисление i-го члена ряда Тейлора: 1 / (i!)
        mpz_mul_ui(factorial, factorial, i);
        mpf_set_z(term, factorial);
        mpf_ui_div(term, 1, term);

        // Суммирование членов ряда
        mpf_add(result, result, term);
    }

    mpz_clear(factorial);
    mpf_clear(term);
}

void compute_sequential_e(int precision, mpf_t result)
{
    mpz_t factorial;
    mpf_t term;
    mpz_init(factorial);
    mpf_init(term);

    mpz_set_ui(factorial, 1);
    mpf_set_ui(result, 1);

    for (int i = 1; i <= 1000; ++i)
    {
        // Вычисление i-го члена ряда Тейлора: 1 / (i!)
        mpz_mul_ui(factorial, factorial, i);
        mpf_set_z(term, factorial);
        mpf_ui_div(term, 1, term);

        // Суммирование членов ряда
        mpf_add(result, result, term);
    }

    mpz_clear(factorial);
    mpf_clear(term);
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2)
    {
        if (rank == 0)
            printf("Usage: %s precision\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int precision = atoi(argv[1]);

    if (precision <= 0)
    {
        if (rank == 0)
            printf("Precision must be a positive integer.\n");
        MPI_Finalize();
        return 1;
    }

    // Установка точности переменных mpf_t
    mpf_set_default_prec(precision * 3.33); // Множитель 3.33 для достаточной точности

    // Вычисление диапазона итераций для каждого процесса
    int chunk_size = 1000 / size;
    int start = rank * chunk_size + 1;
    int end = start + chunk_size - 1;
    if (rank == size - 1)
        end = 1000;

    double start_time, end_time;

    start_time = MPI_Wtime();

    // Вычисление частичной суммы ряда Тейлора для каждого процесса
    mpf_t partial_sum;
    mpf_init(partial_sum);
    compute_partial_sum(start, end, precision, partial_sum);

    // Сбор частичных сумм со всех процессов на процессе с рангом 0
    mpf_t total_sum;
    if (rank == 0)
    {
        mpf_init_set(total_sum, partial_sum);
        mpf_add_ui(total_sum, total_sum, 1); // Прибавляем 1 на процессе с рангом 0
    } else {
        mpf_init(total_sum);
    }

    MPI_Reduce(partial_sum, total_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    // Вывод времени параллельного вычисления
    if (rank == 0)
    {
        printf("Parallel computation time: %f seconds\n", end_time - start_time);
    }

    // Вывод результата с заданной точностью на процессе с рангом 0
    if (rank == 0)
    {
        printf("Parallel calculation:\n");
        printf("e calculated with %d decimal places:\n", precision);
        mpf_out_str(stdout, 10, precision + 1, total_sum);
        printf("\n");

        // Последовательное вычисление для сравнения
        mpf_t sequential_sum;
        mpf_init(sequential_sum);
        
        start_time = clock();
        compute_sequential_e(precision, sequential_sum);
        end_time = clock();

        // Вывод времени последовательного вычисления
        printf("Sequential computation time: %f seconds\n", (end_time - start_time) / CLOCKS_PER_SEC);

        printf("Sequential calculation:\n");
        printf("e calculated with %d decimal places:\n", precision);
        mpf_out_str(stdout, 10, precision + 1, sequential_sum);
        printf("\n");

        // Проверка точности
        if (mpf_cmp(sequential_sum, total_sum) == 0)
        {
            printf("Parallel and sequential results match.\n");
        } else {
            printf("ERROR: Parallel and sequential results do not match.\n");
        }

        mpf_clear(sequential_sum);
    }

    mpf_clear(partial_sum);
    if (rank == 0)
        mpf_clear(total_sum);

    MPI_Finalize();

    return 0;
}