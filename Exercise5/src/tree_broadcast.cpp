#include <mpi.h>
#include <iostream>
#include <cstdlib>

void tree_broadcast(double* data, int n) {
	int l_rank;
	int l_comm_size;

	MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);

	if (l_rank == l_comm_size / 2) {
		if (l_comm_size > 2) {
			MPI_Send(data, n, MPI_DOUBLE, l_rank - 1, 0, MPI_COMM_WORLD);
		}
		MPI_Send(data, n, MPI_DOUBLE, l_rank + 1, 0, MPI_COMM_WORLD);
	} else {
		if (l_rank < l_comm_size / 2) {		
			MPI_Recv(data, n, MPI_DOUBLE, l_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (l_rank > 0) {
				MPI_Send(data, n, MPI_DOUBLE, l_rank - 1, 0, MPI_COMM_WORLD);
			}
		} else {
			MPI_Recv(data, n, MPI_DOUBLE, l_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (l_rank < l_comm_size - 1) {
				MPI_Send(data, n, MPI_DOUBLE, l_rank + 1, 0, MPI_COMM_WORLD);
			}
		}
	}
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int l_rank;
	int l_comm_size;

	MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);
	
	if (l_comm_size < 2) {
		std::cout << "At least two processes are required to run this program!" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	
	int n = std::atoi(argv[1]);
	double* data = new double[n];

	
	if (l_rank == l_comm_size / 2) {			
		srand(l_rank);
		for (int i = 0; i < n; i++) {
			data[i] = (double) rand();
		}
	}
	
	MPI_Barrier( MPI_COMM_WORLD );
	double start_time = MPI_Wtime();
	tree_broadcast(data, n);
	MPI_Barrier( MPI_COMM_WORLD );
	double end_time = MPI_Wtime();
	double elapsed_time = end_time - start_time;
	if (l_rank == l_comm_size / 2) {
		std::cout << "Elapsed time for manual implementation: " << elapsed_time << " seconds" << std::endl;
	}
	
	
	MPI_Barrier( MPI_COMM_WORLD );
	start_time = MPI_Wtime();
	MPI_Bcast(data, n, MPI_DOUBLE, l_comm_size / 2, MPI_COMM_WORLD);
	MPI_Barrier( MPI_COMM_WORLD );
	end_time = MPI_Wtime();
	elapsed_time = end_time - start_time;
	if (l_rank == l_comm_size / 2) {
		std::cout << "Elapsed time for MPI implementation: " << elapsed_time << " seconds" << std::endl;
	}

	MPI_Finalize();

	return 0;
}