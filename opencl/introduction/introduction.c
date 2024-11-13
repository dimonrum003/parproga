#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

void printDeviceInfo(cl_device_id device)
{
    char buffer[1024];
    cl_uint uint_val;
    cl_ulong ulong_val;
    size_t size_val;
    cl_bool bool_val;

    // 1. Имя устройства
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
    printf("Имя устройства: %s\n", buffer);

    // 2. Тип устройства (GPU, CPU и т.д.)
    cl_device_type device_type;
    clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
    printf("Тип устройства: ");
    if (device_type & CL_DEVICE_TYPE_CPU) printf("CPU ");
    if (device_type & CL_DEVICE_TYPE_GPU) printf("GPU ");
    if (device_type & CL_DEVICE_TYPE_ACCELERATOR) printf("Accelerator ");
    printf("\n");

    // 3. Версия OpenCL
    clGetDeviceInfo(device, CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL);
    printf("Версия OpenCL: %s\n", buffer);

    // 4. Версия драйвера
    clGetDeviceInfo(device, CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL);
    printf("Версия драйвера: %s\n", buffer);

    // 5. Размер глобальной памяти
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(ulong_val), &ulong_val, NULL);
    printf("Размер глобальной памяти: %lu MB\n", ulong_val / (1024 * 1024));

    // 6. Размер локальной памяти
    clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(ulong_val), &ulong_val, NULL);
    printf("Размер локальной памяти: %lu KB\n", ulong_val / 1024);

    // 7. Размер кеша памяти (память, доступная внутри вычислительного устройства)
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(ulong_val), &ulong_val, NULL);
    printf("Размер кеша памяти: %lu KB\n", ulong_val / 1024);

    // 8. Максимальное количество вычислительных единиц
    clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(uint_val), &uint_val, NULL);
    printf("Максимальное количество вычислительных единиц: %u\n", uint_val);

    // 9. Размер адресного пространства (максимальная доступная память)
    clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, sizeof(uint_val), &uint_val, NULL);
    printf("Размер адресного пространства: %u-бит\n", uint_val);

    // 10. Максимальный размер рабочей группы
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_val), &size_val, NULL);
    printf("Максимальный размер рабочей группы: %zu\n", size_val);

    // 11. Поддержка изображений (возможность работы с графикой)
    clGetDeviceInfo(device, CL_DEVICE_IMAGE_SUPPORT, sizeof(bool_val), &bool_val, NULL);
    printf("Поддержка изображений: %s\n", bool_val ? "Да" : "Нет");

    // 12. Размер максимальной 2D-изображения
    clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_val), &size_val, NULL);
    printf("Максимальная ширина 2D изображения: %zu\n", size_val);
    clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size_val), &size_val, NULL);
    printf("Максимальная высота 2D изображения: %zu\n", size_val);

    printf("\n");
}

int main()
{
    cl_platform_id platform;
    cl_device_id devices[10];
    cl_uint num_devices;

    // Получение первой доступной платформы
    clGetPlatformIDs(1, &platform, NULL);

    // Получение устройств, поддерживаемых платформой
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 10, devices, &num_devices);

    printf("Найдено устройств: %u\n", num_devices);

    // Печать информации для каждого устройства
    for (cl_uint i = 0; i < num_devices; i++)
    {
        printf("Устройство #%u:\n", i + 1);
        printDeviceInfo(devices[i]);
    }

    return 0;
}
