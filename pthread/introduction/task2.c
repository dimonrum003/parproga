#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 4 // Максимальное количество потоков
#define DEFAULT_N 100 // Значение N по умолчанию

int N; // Количество элементов гармонического ряда
double partial_sum[MAX_THREADS]; // Частичные суммы, посчитанные каждым потоком
pthread_mutex_t mutex_sum; // Мьютекс для защиты доступа к общей сумме

void *compute_partial_sum(void *thread_id) {
    long tid = (long)thread_id;
    int start = tid * (N / MAX_THREADS) + 1; // Начальный индекс для потока
    int end = (tid + 1) * (N / MAX_THREADS); // Конечный индекс для потока
    if (tid == MAX_THREADS - 1) // Если это последний поток, то дополнительно учтем оставшиеся элементы
        end += N % MAX_THREADS;

    double sum = 0.0;
    for (int i = start; i <= end; i++) {
        sum += 1.0 / i;
    }

    pthread_mutex_lock(&mutex_sum);
    partial_sum[tid] = sum;
    pthread_mutex_unlock(&mutex_sum);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s N\n", argv[0]);
        return -1;
    }

    N = atoi(argv[1]);
    if (N <= 0) {
        printf("N must be a positive integer.\n");
        return -1;
    }

    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&mutex_sum, NULL);

    for (long t = 0; t < MAX_THREADS; t++) {
        pthread_create(&threads[t], NULL, compute_partial_sum, (void *)t);
    }

    double total_sum = 0.0;
    for (int t = 0; t < MAX_THREADS; t++) {
        pthread_join(threads[t], NULL);
        total_sum += partial_sum[t];
    }

    printf("Partial sum of harmonic series (1/n) from 1 to %d is: %.10lf\n", N, total_sum);

    pthread_mutex_destroy(&mutex_sum);
    return 0;
}