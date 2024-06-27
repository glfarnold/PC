#include <mpi.h>
#include <iostream>
#include <cstdlib>

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

    MPI_Win l_win;

    double* data = new double[l_comm_size];
    data[0] = 10.0 * l_rank;

    if(l_rank == 0) {
        MPI_Win_create(data, l_comm_size * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);
    } else {
		MPI_Win_create(NULL, 0, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);
	}

    if(l_rank != 0) {
		MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, l_win);
		std::cout << l_rank << " accessing the shared memory space." << std::endl;
        MPI_Put(data, 1, MPI_DOUBLE, 0, l_rank, 1, MPI_DOUBLE, l_win);
        std::cout << l_rank << " freeing the shared memory space." << std::endl;
        MPI_Win_unlock(0, l_win);
    }

    MPI_Win_free(&l_win);

    if(l_rank == 0) {
        for(int i=0; i < l_comm_size; i++) {
            std::cout << " | " << data[i] << " | ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();

    return 0;
}
