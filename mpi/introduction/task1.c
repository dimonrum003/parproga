#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int commsize, my_rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    printf ("Hello World!\tCurrent rank = %d; Total communicators = %d\n", my_rank, commsize);
    MPI_Finalize();
}