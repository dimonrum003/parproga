#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// QuickSort
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
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
void merge(int arr[], int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    
    int L[n1], R[n2];
    
    int i = 0, j = 0;
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    
    i = 0;
    j = 0;
    int k = l;
    
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        
        merge(arr, l, m, r);
    }
}

// Function to generate random array of given size
void generateRandomArray(int arr[], int size)
{
    srand(time(NULL));
    int  i = 0;
    for (i = 0; i < size; i++) {
        arr[i] = rand() % 1000; // Generating random numbers between 0 and 999
    }
}

// Function to generate sorted array of given size
void generateSortedArray(int arr[], int size)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        arr[i] = i;
    }
}

// Function to generate reverse sorted array of given size
void generateReverseSortedArray(int arr[], int size)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        arr[i] = size - i;
    }
}

int main()
{
    int size_arr[3] = {1000, 5000, 20000};
    int size = 0;
    int i = 0;
    for (i = 0; i < sizeof(size_arr)/sizeof(size_arr[0]); i++)
    {
        size = size_arr[i];
        printf("Array size: %d\n", size);

        // Best case
        int bestCaseArr[size];
        generateSortedArray(bestCaseArr, size);

        clock_t start, end;
        double cpu_time_used;

        // QuickSort best case
        start = clock();
        quickSort(bestCaseArr, 0, size - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("QuickSort (best case   ): %f seconds\n", cpu_time_used);

        // MergeSort best case
        start = clock();
        mergeSort(bestCaseArr, 0, size - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("MergeSort (best case   ): %f \n", cpu_time_used);

        // Average case
        int avgCaseArr[size];
        generateRandomArray(avgCaseArr, size);

        // QuickSort average case
        start = clock();
        quickSort(avgCaseArr, 0, size - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("QuickSort (average case): %f \n", cpu_time_used);

        // MergeSort average case
        start = clock();
        mergeSort(avgCaseArr, 0, size - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("MergeSort (average case): %f \n", cpu_time_used);

        // Worst case
        int worstCaseArr[size];
        generateReverseSortedArray(worstCaseArr, size);

        // QuickSort worst case
        start = clock();
        quickSort(worstCaseArr, 0, size - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("QuickSort (worst case  ): %f \n", cpu_time_used);

        // MergeSort worst case
        start = clock();
        mergeSort(worstCaseArr, 0, size - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("MergeSort (worst case  ): %f \n", cpu_time_used);

        printf("\n");
    }

    return 0;
}