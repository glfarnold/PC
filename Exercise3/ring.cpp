#include <mpi.h>
#include <iostream>
#include <cassert>

int main(int argc, char** argv) {

  int l_ret = MPI_Init(&argc, &argv);
  assert(l_ret == MPI_SUCCESS); // Ensure MPI_Init succeeded

  int l_rank;
  int l_comm_size;

  MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);

  if(l_comm_size < 2){
    std::cout << "Requires a minimum of two processes!" << std::endl;
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  if(l_rank == 0){
    int my_val = 42;
    int receiver_rank = 1;
    l_ret = MPI_Send(&my_val, 1, MPI_INT, receiver_rank, 0, MPI_COMM_WORLD);
    assert(l_ret == MPI_SUCCESS);
    std::cout << "Process " << l_rank << " sent value " << my_val << " to process " << receiver_rank << std::endl;
    
    int received_value;
    int sender_rank = l_comm_size - 1;  
    int message_tag = 0;
    MPI_Status status;
    l_ret = MPI_Recv(&received_value, 1, MPI_INT, sender_rank, message_tag, MPI_COMM_WORLD, &status); 
    assert(l_ret == MPI_SUCCESS);
    std::cout << "Process " << l_rank << " received value " << received_value << " from process " << status.MPI_SOURCE << std::endl;
  }
  else if(l_rank == l_comm_size - 1){
    int received_value;
    int sender_rank = l_rank - 1;  
    int message_tag = 0;
    MPI_Status status;
    l_ret = MPI_Recv(&received_value, 1, MPI_INT, sender_rank, message_tag, MPI_COMM_WORLD, &status);
    assert(l_ret == MPI_SUCCESS);
    std::cout << "Process " << l_rank << " received value " << received_value << " from process " << status.MPI_SOURCE << std::endl;

    int receiver_rank = 0;
    received_value += l_rank;
    l_ret = MPI_Send(&received_value, 1, MPI_INT, receiver_rank, 0, MPI_COMM_WORLD);
    assert(l_ret == MPI_SUCCESS);
    std::cout << "Process " << l_rank << " sent value " << received_value << " to process " << receiver_rank << std::endl;
  }
  else{
    int received_value;
    int sender_rank = l_rank - 1;  
    int message_tag = 0;
    MPI_Status status;
    l_ret = MPI_Recv(&received_value, 1, MPI_INT, sender_rank, message_tag, MPI_COMM_WORLD, &status);
    assert(l_ret == MPI_SUCCESS);
    std::cout << "Process " << l_rank << " received value " << received_value << " from process " << status.MPI_SOURCE << std::endl;

    int receiver_rank = l_rank + 1;
    l_ret = MPI_Send(&received_value, 1, MPI_INT, receiver_rank, 0, MPI_COMM_WORLD);
    assert(l_ret == MPI_SUCCESS);
    std::cout << "Process " << l_rank << " sent value " << received_value << " to process " << receiver_rank << std::endl;
  }

  MPI_Finalize();

  return 0;
}
