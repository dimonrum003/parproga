#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4// Количество потоков, которые мы хотим создать

void *thread_function(void *thread_id) {
    long tid = (long)thread_id; // Идентификатор потока, приведенный к long

    printf("Hello World from thread %ld out of %d\n", tid, NUM_THREADS);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS]; // Массив для хранения идентификаторов потоков
    int rc;
    long t;

    for(t = 0; t < NUM_THREADS; t++) {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)t); // Создание потока
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_exit(NULL);
}