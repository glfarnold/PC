#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Determine color based on even or odd rank
  int color = rank / 4;

  // Split the communicator based on color
  MPI_Comm new_comm;
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);

  int new_rank, new_size;
  MPI_Comm_rank(new_comm, &new_rank);
  MPI_Comm_size(new_comm, &new_size);

  // Print information about the original and new communicators
  std::cout << "Rank " << rank << " out of " << size << " in MPI_COMM_WORLD, " \
            << "New Rank " << new_rank << " out of " << new_size << " in new_comm, " \
            << "Subgroup " << color << std::endl;


  MPI_Comm_free(&new_comm);

  MPI_Finalize();
  return 0;
}
