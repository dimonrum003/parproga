#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

#define MESSAGE_SIZE 1024 // 1024000

int main(int argc, char** argv)
{
    int rank, size;
    char message[MESSAGE_SIZE];
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2)
    {
        printf("This program requires exactly 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        sprintf(message, "Hello, process 1!");

        start_time = MPI_Wtime();
        MPI_Send(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD);     // 0.000036 , 2.001467 seconds
        // MPI_Ssend(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD); // 2.000016 , 2.001424
        // MPI_Rsend(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD); // 0.000032 , 2.001315
        // MPI_Bsend(message, MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD); // 0.000040 , error
        end_time = MPI_Wtime();

        printf("Time taken for MPI_Send: %f seconds\n", end_time - start_time);
    } else if (rank == 1) {
        sleep(2); // Задержка в 2 секунды

        MPI_Recv(message, MESSAGE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Received message: %s\n", message);
    }

    MPI_Finalize();
    return 0;
}
