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
    
    int data = 0;

    if(l_rank == 0) {        
        srand(time(NULL));
        data = rand();
        std::cout << "Data on rank " << l_rank << " : " << data << std::endl;
        MPI_Win_create(&data, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);
    }

    if(l_rank == 1) {
		std::cout << "Data on rank " << l_rank << " : " << data << std::endl;
		MPI_Win_create(NULL, 0, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);      
    }
    
    MPI_Win_fence(MPI_MODE_NOPRECEDE, l_win);
    
    if(l_rank == 1) {
		MPI_Get(&data, 1, MPI_INT, 0, 0, 1, MPI_INT, l_win);
        
    }  
	
    MPI_Win_fence(0, l_win);

    MPI_Win_free(&l_win);

    std::cout << "Data on rank " << l_rank << " : " << data << std::endl;

    MPI_Finalize();

    return 0;
}
