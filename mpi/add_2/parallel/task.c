#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// ### QuickSort:
// - Лучший случай:  Лучший случай для QuickSort наступает, когда массив уже отсортирован. 
//                   В этом случае, каждый разделенный массив будет иметь примерно одинаковый размер, что позволяет алгоритму работать эффективно. 
//                   Сложность лучшего случая QuickSort - O(n log n).
// - Средний случай: Средний случай для QuickSort наступает при случайном распределении элементов в массиве. 
//                   В этом случае, QuickSort обычно работает эффективно, разделяя массив на примерно равные части. 
//                   Сложность среднего случая QuickSort также - O(n log n).
// - Худший случай:  Худший случай для QuickSort наступает, когда массив уже отсортирован в обратном порядке или содержит множество повторяющихся элементов.
//                   В этом случае, QuickSort может работать медленно из-за неэффективного выбора опорного элемента. 
//                   Сложность худшего случая QuickSort - O(n^2).

// ### MergeSort:
//                  Aлгоритм всегда выполняется за O(n log n), независимо от распределения элементов в массиве.
//---------------------------------------------------------------------------------------------------------------------------------------------------------------

// QuickSort
void quickSort(int arr[], int low, int high)
{
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        
        int j = 0;
        for (j = low; j <= high - 1; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        
        int pi = i + 1;
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// MergeSort
void merge(int *arr, int *temp, int left, int mid, int right)
{
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid)
    {
        temp[k++] = arr[i++];
    }

    while (j <= right)
    {
        temp[k++] = arr[j++];
    }

    for (i = left; i <= right; i++)
    {
        arr[i] = temp[i];
    }
}

void mergeSort(int *arr, int *temp, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, temp, left, mid);
        mergeSort(arr, temp, mid + 1, right);

        merge(arr, temp, left, mid, right);
    }
}

// Function to generate random array of given size
void generateRandomArray(int arr[], int size)
{
    srand(time(NULL));
    int i = 0;
    for (i = 0; i < size; i++)
    {
        arr[i] = rand() % 1000; // Generating random numbers between 0 and 999
    }
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n, name;
    if (argc > 2)
    {
        name = atoi(argv[1]);
        n = atoi(argv[2]);
    }
   
    // int n = 100;
    int *arr = (int*)malloc(n * sizeof(int));
    int *temp = (int*)malloc(n * sizeof(int));

    double start_time = 0, end_time = 0;

    if (rank == 0)
    {
        if (name == 0) printf("Arr_size = %d, Name = Merge, Comm_size = %d\n", n, size);
        else           printf("Arr_size = %d, Name = Quick, Comm_size = %d\n", n, size);
        generateRandomArray(arr, n);
        start_time = MPI_Wtime();
    }

    int local_n = n / size;
    int *local_arr = (int*)malloc(local_n * sizeof(int));

    int i = 0;

    //Перед MPI_Scatter
    // if (n == 5) { 
    //     printf("Rank %d before MPI_Scatter: ", rank);  
    //     for (i = 0; i < local_n; i++) {
    //         printf("%d ", local_arr[i]);
    //     }
    //     printf("\n");
    // }

    MPI_Scatter(arr, local_n, MPI_INT, local_arr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // После MPI_Scatter
    // if (n == 5) { 
    //     printf("Rank %d after MPI_Scatter: ", rank);
    //     for (i = 0; i < local_n; i++) {
    //         printf("%d ", local_arr[i]);
    //     }
    //     printf("\n");
    // }

    if (name == 0)
    {
        mergeSort(local_arr, temp, 0, local_n - 1);
    } else {
        quickSort(local_arr, 0, local_n - 1);
    }

    // После сортировки
    // printf("Rank %d after sorting: ", rank);
    // for (i = 0; i < local_n; i++) {
    //     printf("%d ", local_arr[i]);
    // }
    // printf("\n");

    MPI_Gather(local_arr, local_n, MPI_INT, arr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // После MPI_Gather
    if (rank == 0)
    {
        // if (n == 5) {
        //     printf("Rank %d after MPI_Gather: ", rank);
        //     for (i = 0; i < n; i++) {
        //         printf("%d ", arr[i]);
        //     }
        //     printf("\n");
        // }
        if (name == 0)
            mergeSort(arr, temp, 0, n - 1);
        else
            quickSort(arr, 0, n - 1);
        
        end_time = MPI_Wtime();
        double sort_time = end_time - start_time;
        printf("Sort time: %f \n", sort_time);
    }

    free(local_arr);
    free(arr);
    free(temp);

    MPI_Finalize();

    return 0;
}
