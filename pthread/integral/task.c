#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

// PS: компилировать с флагами "-pthread -lm"

typedef struct
{
    double a;
    double b;
    double result;
    double eps;
    double (*func)(double);
} ThreadData;

double func(double x)
{
    return sin(1/(x*x));
}

void *integrate(void *arg);
double simpson_integration(double a, double b, double eps, double (*func)(double));
double adaptive_trapezoidal(double a, double b, double eps, double (*func)(double));

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <num_threads> <epsilon>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    double eps = atof(argv[2]);
    double a = 0.001;
    double b = 1.0;

    // Измерение времени выполнения последовательной версии
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    double sequential_result = simpson_integration(a, b, eps, func);
    clock_gettime(CLOCK_REALTIME, &end);
    double sequential_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Sequential result: \t%.10f, Time: %.6f seconds\n", sequential_result, sequential_time);

    // Измерение времени выполнения параллельной версии
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = (ThreadData *)malloc(num_threads * sizeof(ThreadData));
    double parallel_result = 0.0;

    clock_gettime(CLOCK_REALTIME, &start);

    // Разделяем диапазон на равные части для каждого потока
    double range = b - a;
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].a = a + i * range / num_threads;
        thread_data[i].b = a + (i + 1) * range / num_threads;
        thread_data[i].result = 0.0;
        thread_data[i].eps = eps / num_threads; // Делим погрешность на количество потоков
        thread_data[i].func = func;
        pthread_create(&threads[i], NULL, integrate, (void *)&thread_data[i]);
    }

    // Ждем завершения всех потоков
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        parallel_result += thread_data[i].result;
    }

    clock_gettime(CLOCK_REALTIME, &end);
    double parallel_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Parallel result: \t%.10f, Time: %.6f seconds\n", parallel_result, parallel_time);

    free(threads);
    free(thread_data);

    return 0;
}

void *integrate(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    data->result = adaptive_trapezoidal(data->a, data->b, data->eps, data->func);
    pthread_exit(NULL);
}

double adaptive_trapezoidal(double a, double b, double eps, double (*func)(double))
{
    double result = 0.0;
    double step = (b - a) / 2.0;
    double previous_result;
    double current_result = (func(a) + func(b)) * step / 2.0;

    do
    {
        previous_result = current_result;
        current_result = 0.0;
        for (double x = a + step / 2; x < b; x += step)
        {
            current_result += func(x);
        }
        current_result = (previous_result + step * current_result) / 2.0;
        step /= 2.0;
    } while (fabs(current_result - previous_result) > eps);

    return current_result;
}

double simpson_integration(double a, double b, double eps, double (*func)(double))
{
    int n = 2; // Начальное количество интервалов
    double h = (b - a) / n;
    double result = (func(a) + 4*func(a + h) + func(b)) * h / 3.0;
    double previous_result;
    
    do
    {
        previous_result = result;
        n *= 2;
        h = (b - a) / n;
        result = 0.0;
        for (int i = 0; i <= n; i++)
        {
            if (i == 0 || i == n)
            {
                result += func(a + i * h);
            } else if (i % 2 == 0) {
                result += 2 * func(a + i * h);
            } else {
                result += 4 * func(a + i * h);
            }
        }
        result *= h / 3.0;
    } while (fabs(result - previous_result) > eps);

    return result;
}