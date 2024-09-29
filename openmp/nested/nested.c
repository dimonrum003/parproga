#include <stdio.h>
#include <omp.h>

int main()
{
    // Включаем вложенный параллелизм
    omp_set_nested(1);  // 1 — разрешаем вложенный параллелизм, 0 — отключаем

    // Внешний параллельный регион (уровень 1)
    #pragma omp parallel num_threads(4)
    {
        int thread_id_level_1 = omp_get_thread_num();
        int num_threads_level_1 = omp_get_num_threads();
        
        printf("Level 1: Thread %d out of %d threads\n", thread_id_level_1, num_threads_level_1);
        
        // Внутренний параллельный регион (уровень 2)
        #pragma omp parallel num_threads(3)
        {
            int thread_id_level_2 = omp_get_thread_num();
            int num_threads_level_2 = omp_get_num_threads();

            printf("Level 2: Thread %d out of %d threads (parent thread: %d)\n",
                   thread_id_level_2, num_threads_level_2, thread_id_level_1);

            // Внутренний параллельный регион (уровень 3)
            #pragma omp parallel num_threads(2)
            {
                int thread_id_level_3 = omp_get_thread_num();
                int num_threads_level_3 = omp_get_num_threads();

                printf("Level 3: Thread %d out of %d threads (parent thread: %d, grandparent thread: %d)\n",
                       thread_id_level_3, num_threads_level_3, thread_id_level_2, thread_id_level_1);
            }
        }
    }

    return 0;
}
