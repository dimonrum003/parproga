#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

int shared_variable = 0; // Глобальная переменная, к которой будут обращаться потоки
pthread_mutex_t mutex; // Мьютекс для защиты доступа к глобальной переменной

void *thread_function(void *thread_id) {
    long tid = (long)thread_id;

    pthread_mutex_lock(&mutex); // Захватываем мьютекс перед доступом к общей переменной
    shared_variable++; // Изменяем значение глобальной переменной
    printf("Thread %ld: Shared variable is %d\n", tid, shared_variable);
    pthread_mutex_unlock(&mutex); // Освобождаем мьютекс после доступа к общей переменной

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    pthread_mutex_init(&mutex, NULL); // Инициализируем мьютекс

    for(t = 0; t < NUM_THREADS; t++) {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)t);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        pthread_join(threads[t], NULL); // Ждем завершения каждого потока перед созданием следующего
    }

    pthread_mutex_destroy(&mutex); // Уничтожаем мьютекс после использования

    return 0;
}