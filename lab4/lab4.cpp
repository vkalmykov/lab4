#include "mpi.h"
#include <cstdio>
#pragma comment (lib, "mpich.lib")

int main(int argc, char * argv[]) {
	const int bufferSize = 256;
	int rank, size, resultlen;
	double startTime;
	char name[MPI_MAX_PROCESSOR_NAME], sendData[bufferSize], recvData[bufferSize];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(name, &resultlen);
	
	if (rank == 0) {
		printf("Enter data: ");
		fflush(stdout);
		scanf("%s", &sendData);
		MPI_Send(&sendData, bufferSize, MPI_CHAR, rank + 1, rank + 1, MPI_COMM_WORLD);
		printf("Sending data...\n");
		fflush(stdout);
		startTime = MPI_Wtime();
		MPI_Recv(&recvData, bufferSize, MPI_CHAR, size - 1, 0, MPI_COMM_WORLD, &status);
		printf("Process %d recieved data from proccess %d. Total time: %5.6f\n", rank, size-1, MPI_Wtime() - startTime);
		fflush(stdout);
	}
	else if(rank == size-1) {
		MPI_Recv(&recvData, bufferSize, MPI_CHAR, size - 2, rank, MPI_COMM_WORLD, &status);
		printf("Proccess %d recieved data from proccess %d...\n", rank, size - 2);
		fflush(stdout);
		MPI_Send(&recvData, bufferSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	else {
		MPI_Recv(&recvData, bufferSize, MPI_CHAR, rank-1, rank, MPI_COMM_WORLD, &status);
		printf("Proccess %d recieved data from proccess %d...\n", rank, rank-1);
		fflush(stdout);
		MPI_Send(&recvData, bufferSize, MPI_CHAR, rank + 1, rank + 1, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}