#include <mpi.h>
#include <iostream>
#include <cassert>
#include <cstdlib>

void my_custom_reduction(){
    int rank;
    int comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if(comm_size < 2){
        std::cout << "Minimum of two processes needed!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if(rank == 0){
        double* value_array = new double[comm_size-1];
        MPI_Request* request_array = new MPI_Request[comm_size-1];
    
        for(int i=0; i < comm_size-1; i++){
            int l_ret = MPI_Irecv(&value_array[i], 1, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD, &request_array[i]);
            assert(l_ret == MPI_SUCCESS);
        }
        int l_ret = MPI_Waitall(comm_size-1, request_array, MPI_STATUSES_IGNORE);
        assert(l_ret == MPI_SUCCESS);

        double sum = 0;
        for(int i=0; i < comm_size-1; i++){    
            std::cout << "| "<< value_array[i] << " |";
            sum += value_array[i];
        }
        std::cout << std::endl << "sum: " << sum << std::endl;
    }else{
        srand(rank);
        double data = (double)rand();
        MPI_Request send_request;
        
        for(int i=0; i < comm_size-1; i++){
            int l_ret = MPI_Isend(&data, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &send_request);
            assert(l_ret == MPI_SUCCESS);
        }
    }
}

int main(int argc, char** argv){
    int l_ret = MPI_Init(&argc, &argv);
    assert(l_ret == MPI_SUCCESS);

    my_custom_reduction();

    MPI_Finalize();
}