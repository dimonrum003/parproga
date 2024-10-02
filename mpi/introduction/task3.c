#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int message = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Круговая пересылка
    if (rank == 0)
    {
        message++; // Произвольное действие
        printf("Rank %d send message: %d\n", rank, message);
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&message, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received message: %d\n", rank, message);
    } else {
        MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received message: %d\n", rank, message);
        message++; // Произвольное действие
        if (rank < size - 1)
        {
            printf("Rank %d send message: %d\n", rank, message);
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);     
        } else {
            printf("Rank %d send message: %d\n", rank, message);
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}