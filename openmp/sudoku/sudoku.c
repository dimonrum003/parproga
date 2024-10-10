#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#define SIZE 9    // Размер поля
#define SUBGRID 3 // Размер малых квадратов

int grid[SIZE][SIZE]; // Поле Судоку

// Функция для печати поля
void print_grid()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%2d ", grid[i][j]);
        }
        printf("\n");
    }
}

// Функция для чтения поля Судоку из текстового файла
void read_grid(const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        exit(1);
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fscanf(file, "%d", &grid[i][j]);
        }
    }
    fclose(file);
}

// Функция для проверки, можно ли поставить число num в клетку (row, col)
bool is_valid(int row, int col, int num)
{
    // Проверка строки
    for (int x = 0; x < SIZE; x++)
    {
        if (grid[row][x] == num)
        {
            return false;
        }
    }

    // Проверка столбца
    for (int x = 0; x < SIZE; x++) 
    {
        if (grid[x][col] == num)
        {
            return false;
        }
    }

    // Проверка подгруппы 5x5
    int startRow = row / SUBGRID * SUBGRID;
    int startCol = col / SUBGRID * SUBGRID;
    for (int i = 0; i < SUBGRID; i++)
    {
        for (int j = 0; j < SUBGRID; j++)
        {
            if (grid[startRow + i][startCol + j] == num)
            {
                return false;
            }
        }
    }

    return true;
}

// Последовательный алгоритм решения Судоку (backtracking)
bool solve_sudoku()
{
    int row = -1, col = -1;
    bool is_empty = false;

    // Поиск незаполненной клетки
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (grid[i][j] == 0)
            {
                row = i;
                col = j;
                is_empty = true;
                break;
            }
        }
        if (is_empty)
        {
            break;
        }
    }

    // Если пустых клеток нет, значит решено
    if (!is_empty)
    {
        return true;
    }

    // Пробуем вставить числа
    for (int num = 1; num <= SIZE; num++)
    {
        if (is_valid(row, col, num))
        {
            grid[row][col] = num;

            // Рекурсивный вызов для дальнейшего решения
            if (solve_sudoku())
            {
                return true;
            }

            // Если решение не найдено, откатываемся назад
            grid[row][col] = 0;
        }
    }

    return false;
}

// Параллельный алгоритм решения Судоку с использованием OpenMP
bool parallel_solve_sudoku()
{
    int row = -1, col = -1;
    bool is_empty = false;
    bool solved = false;  // Флаг для отслеживания состояния решения

    // Поиск незаполненной клетки
    #pragma omp parallel for collapse(2) reduction(||:is_empty)
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (grid[i][j] == 0)
            {
                row = i;
                col = j;
                is_empty = true;
            }
        }
    }

    // Если пустых клеток нет, задача решена
    if (!is_empty)
    {
        return true;
    }

    // Пробуем вставить числа с использованием задач OpenMP
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int num = 1; num <= SIZE; num++)
            {
                // Если решение найдено, прекращаем создание задач
                if (!solved)
                {
                    #pragma omp task firstprivate(num, row, col) shared(solved)
                    {
                        if (!solved && is_valid(row, col, num))
                        {
                            grid[row][col] = num;

                            // Рекурсивный вызов параллельной функции
                            if (parallel_solve_sudoku())
                            {
                                // Атомарное обновление флага, если решение найдено
                                #pragma omp atomic write
                                solved = true;
                            }

                            // Откат изменений, если решение не найдено
                            if (!solved)
                            {
                                grid[row][col] = 0;
                            }
                        }
                    }
                }
            }
            #pragma omp taskwait
        }
    }

    return solved;
}

// Копирование содержимого одного массива в другой
void copy_grid(int src[SIZE][SIZE], int dest[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

// Главная функция программы
int main()
{
    // Чтение поля Судоку
    read_grid("sudoku_inputs.txt");

    // Копирование исходного поля для повторного использования
    int original_grid[SIZE][SIZE];
    copy_grid(grid, original_grid);

    // *** Последовательное решение ***
    printf("Решение последовательного алгоритма...\n");
    double start_time_seq = omp_get_wtime();  // Начало измерения времени
    if (solve_sudoku())
    {
        double end_time_seq = omp_get_wtime();  // Конец измерения времени
        printf("Решённое поле:\n");
        print_grid();
        printf("Время, затраченное на последовательное решение: %f секунд\n", end_time_seq - start_time_seq);
    } else {
        printf("Последовательный алгоритм не нашел решения.\n");
    }

    // Восстановление исходного состояния поля для параллельного решения
    copy_grid(original_grid, grid);

    // *** Параллельное решение ***
    printf("Решение параллельного алгоритма...\n");
    double start_time_par = omp_get_wtime();  // Начало измерения времени
    #pragma omp parallel
    {
        #pragma omp single
        {
            if (parallel_solve_sudoku())
            {
                double end_time_par = omp_get_wtime();  // Конец измерения времени
                printf("Решённое поле (параллельное решение):\n");
                print_grid();
                printf("Время, затраченное на параллельное решение: %f секунд\n", end_time_par - start_time_par);
            } else {
                printf("Параллельный алгоритм не нашел решения.\n");
            }
        }
    }

    return 0;
}