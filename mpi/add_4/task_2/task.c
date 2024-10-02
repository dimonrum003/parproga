#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size, N;
    double sum_partial = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2)
    {
        if (rank == 0)
            printf("Использование: %s N\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    N = atoi(argv[1]);

    int start = rank * (N / size) + 1;
    int end = (rank + 1) * (N / size);
    if (rank == size - 1)
    {
        end = N;
    }

    for (int i = start; i <= end; i++)
    {
        sum_partial += 1.0 / i;
    }

    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, "output.txt", MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &file);
    
    char buffer[100];
    int offset = sprintf(buffer, "Rank %d: %.6f\n", rank, sum_partial);

    MPI_Offset file_size;
    MPI_File_get_size(file, &file_size);

    MPI_Offset total_offset = file_size - (rank + 1) * offset;

    MPI_File_seek(file, total_offset, MPI_SEEK_SET);
    MPI_File_write(file, buffer, offset, MPI_CHAR, MPI_STATUS_IGNORE);

    MPI_File_close(&file);

    MPI_Finalize();
    return 0;
}