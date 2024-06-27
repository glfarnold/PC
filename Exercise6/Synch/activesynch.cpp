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
    MPI_Group l_comm_group;
    MPI_Comm_group(MPI_COMM_WORLD, &l_comm_group);
    
    MPI_Group l_group_incl;
    int l_ranks_target[1] = {0};
    int* l_ranks_origin = new int[l_comm_size - 1];
    for (int i = 1; i < l_comm_size; i++) {
		l_ranks_origin[i - 1] = i;
	}
	
	double* data = new double[l_comm_size];
    data[0] = 10.0 * l_rank;
	
	if(l_rank == 0) {
		MPI_Win_create(data, l_comm_size * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);
        MPI_Group_incl(l_comm_group, l_comm_size - 1, l_ranks_origin, &l_group_incl);
    } else {
		MPI_Win_create(NULL, 0, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);
		MPI_Group_incl(l_comm_group, 1, l_ranks_target, &l_group_incl);
	}

    if(l_rank == 0) {
        MPI_Win_post(l_group_incl, 0, l_win);
        MPI_Win_wait(l_win);
    } else {
		MPI_Win_start(l_group_incl, 0, l_win);
		MPI_Put(data, 1, MPI_DOUBLE, 0, l_rank, 1, MPI_DOUBLE, l_win);
		MPI_Win_complete(l_win);
	}

    MPI_Win_free(&l_win);
    MPI_Group_free(&l_comm_group);
    MPI_Group_free(&l_group_incl);

    if(l_rank == 0) {
        for(int i=0; i < l_comm_size; i++) {
            std::cout << " | " << data[i] << " | ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();

    return 0;
}
