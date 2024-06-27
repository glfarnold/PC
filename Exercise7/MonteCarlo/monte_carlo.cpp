#include <iostream>
#include <random>
#include <cmath>
#include <mpi.h>

double** number_generation(int n_samples, int l_rank) {
    std::mt19937 gen(l_rank);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double** result = new double*[n_samples];

    for(int i=0; i < n_samples; i++){
        result[i] = new double[2];
        result[i][0] = dis(gen);
        result[i][1] = dis(gen);
    }
	
    return result;
}

int pi_estimation(int n_samples, double** random_numbers) {
    int count = 0;
    double square_x = 0;
    double square_y = 0;
    
    for(int i=0; i < n_samples; i++){
        square_x = pow(random_numbers[i][0], 2);
        square_y = pow(random_numbers[i][1], 2); 

        if(square_x + square_y < 1){
            count++;
        }
    }

    return count;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

	int l_rank;
	int l_comm_size;

	MPI_Comm_rank(MPI_COMM_WORLD, &l_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &l_comm_size);
	
	double start_time = MPI_Wtime();
	
	int n_samples = 50000000 / l_comm_size;
	n_samples += (l_rank == 0) ? 50000000 % l_comm_size : 0;
	double** numbers = number_generation(n_samples, l_rank);
    int pi_count = pi_estimation(n_samples, numbers);

    MPI_Win l_win;

    int* pi_counts = new int[l_comm_size];

    if(l_rank == 0){
        pi_counts[0] = pi_count;
        MPI_Win_create(pi_counts, sizeof(pi_counts), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);
    } else {
		MPI_Win_create(NULL, 0, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &l_win);
	}
    
    MPI_Win_fence(MPI_MODE_NOPRECEDE, l_win);

    if(l_rank != 0){
        MPI_Put(&pi_count, 1, MPI_INT, 0, l_rank, 1, MPI_INT, l_win);
    }

    MPI_Win_fence(0, l_win);
    MPI_Win_free(&l_win);

    if(l_rank == 0){
        int sum = 0;
        for(int i=0; i < l_comm_size; i++){
            sum += pi_counts[i];
        }
        double pi = 4 * ((double) sum) / 50000000;
        std::cout << "Approximation of Pi: " << pi << std::endl;
        std::cout << "Error: " << (pi - M_PI) << std::endl;
    }
	
	double end_time = MPI_Wtime();
	double elapsed_time = end_time - start_time;

	if (l_rank == 0) {
		std::cout << "Elapsed time: " << elapsed_time << " seconds" << std::endl;
	}
	
    MPI_Finalize();

	delete[] numbers;
    delete[] pi_counts;
}
