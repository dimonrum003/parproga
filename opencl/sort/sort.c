#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// Функция для вывода массива
void printArray(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

// Функция проверки отсортированного массива
int checkArray(int *array, int size)
{
    for (int i = 1; i < size; i++)
    {
        if (array[i] < array[i-1])
        {
            printf("Ошибка в сортировке\n");
            return 1;
        }
    }

    printf("Отсортировано правильно\n");
    return 0;
}

// Функция для вычисления ближайшей степени двойки, больше или равной числу n
int nextPowerOfTwo(int n)
{
    int power = 1;
    while (power < n)
    {
        power *= 2;
    }
    return power;
}

// OpenCL кернел для параллельного слияния подмассивов
const char *kernelSource = R"CLC(
__kernel void merge(__global int *data, __global int *temp, int width, int size)
{
    int gid = get_global_id(0);
    int start = gid * 2 * width;
    int mid = start + width;
    int end = start + 2 * width;

    if (mid > size) mid = size;
    if (end > size) end = size;

    int i = start;
    int j = mid;
    int k = start;

    while (i < mid && j < end)
    {
        if (data[i] <= data[j])
        {
            temp[k++] = data[i++];
        } else
        {
            temp[k++] = data[j++];
        }
    }

    while (i < mid)
    {
        temp[k++] = data[i++];
    }

    while (j < end)
    {
        temp[k++] = data[j++];
    }

    for (i = start; i < end; i++)
    {
        data[i] = temp[i];
    }
}
)CLC";

int main(int argc, char *argv[])
{
    int array_size;
    if (argc > 1)
    {
        array_size = atoi(argv[1]);
        if (array_size <= 0)
        {
            printf("Недопустимый размер массива\n");
            return 1;
        }
    } else 
    {
        printf("Использование: %s <ARRAY_SIZE>\n", argv[0]);
        return 1;
    }

    // Определение размера, кратного степени двойки
    int padded_size = nextPowerOfTwo(array_size);
    int *data = (int *)malloc(sizeof(int) * padded_size);
    int *temp = (int *)malloc(sizeof(int) * padded_size);

    // Инициализация массива случайными числами и заполнение пустых мест большими числами
    srand(time(NULL));
    for (int i = 0; i < array_size; i++)
    {
        data[i] = rand() % 100;
    }
    for (int i = array_size; i < padded_size; i++)
    {
        data[i] = INT_MAX;
    }

    printf("Изначальный массив:\n");
    printArray(data, array_size);

    // Инициализация OpenCL
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem buffer_data, buffer_temp;

    cl_int err;
    err = clGetPlatformIDs(1, &platform, NULL);
    err |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    queue = clCreateCommandQueue(context, device, 0, &err);

    // Создание буферов и копирование данных
    buffer_data = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * padded_size, NULL, &err);
    buffer_temp = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * padded_size, NULL, &err);
    err = clEnqueueWriteBuffer(queue, buffer_data, CL_TRUE, 0, sizeof(int) * padded_size, data, 0, NULL, NULL);

    // Компиляция программы и создание кернела
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    if (err != CL_SUCCESS)
    {
        // Печать ошибки компиляции, если она произошла
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char *)malloc(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Ошибка компиляции кернела:\n%s\n", log);
        free(log);
        return 1;
    }

    kernel = clCreateKernel(program, "merge", &err);

    // Параметры для сортировки слиянием
    int width = 1;
    
    // Измерение времени начала сортировки
    clock_t start_time = clock();

    while (width < padded_size)
    {
        // Установка аргументов кернела
        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_data);
        err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_temp);
        err |= clSetKernelArg(kernel, 2, sizeof(int), &width);
        err |= clSetKernelArg(kernel, 3, sizeof(int), &padded_size);

        // Запуск кернела
        size_t global_work_size = padded_size / (2 * width);
        err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
        clFinish(queue);

        // Увеличение размера слияния для следующей итерации
        width *= 2;
    }

    // Измерение времени конца сортировки
    clock_t end_time = clock();

    // Чтение данных обратно
    err = clEnqueueReadBuffer(queue, buffer_data, CL_TRUE, 0, sizeof(int) * padded_size, data, 0, NULL, NULL);

    // Вывод отсортированного массива, исключая добавленные элементы
    printf("Отсортированный массив:\n");
    printArray(data, array_size);

    // Проверяем отсортированный массив
    checkArray(data, array_size);

    // Вычисление и вывод времени выполнения сортировки
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Время выполнения сортировки: %f секунд\n", time_taken);

    // Освобождение ресурсов
    clReleaseMemObject(buffer_data);
    clReleaseMemObject(buffer_temp);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(data);
    free(temp);

    return 0;
}