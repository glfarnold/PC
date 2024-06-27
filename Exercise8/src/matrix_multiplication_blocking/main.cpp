#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <math.h>

void impl_blocked_k(double* i_A,
                    double* i_b,
                    double* io_c,
                    std::size_t i_k,
                    std::size_t i_m,
                    std::size_t i_blockSize) {

    std::size_t l_nBlocks = i_k / i_blockSize;
    for( std::size_t l_bk = 0; l_bk < l_nBlocks; l_bk++ )
        for( std::size_t l_m = 0; l_m < i_m; l_m++ )
            for( std::size_t l_k = l_bk*i_blockSize; l_k < (l_bk+1)*i_blockSize; l_k++)
                io_c[l_m] += i_A[l_m*i_k + l_k] * i_b[l_k];
}


void impl_blocked_m(double* i_A,
                    double* i_b,
                    double* io_c,
                    std::size_t i_k,
                    std::size_t i_m,
                    std::size_t i_blockSize) {

    std::size_t l_nBlocks = i_m / i_blockSize;
    for( std::size_t l_bk = 0; l_bk < l_nBlocks; l_bk++ )
        for( std::size_t l_k = 0; l_k < i_k; l_k++ )
            for( std::size_t l_m = l_bk*i_blockSize; l_m < (l_bk+1)*i_blockSize; l_m++)
                io_c[l_m] += i_A[l_m*i_k + l_k] * i_b[l_k];
}

void matmul_km(double* i_A,
                double* i_b,
                double* io_c,
                std::size_t i_m,
                std::size_t i_k ) {

    for( std::size_t l_k = 0; l_k < i_k; l_k++ )
        for( std::size_t l_m = 0; l_m < i_m; l_m++ )
            io_c[l_k] += i_A[l_k*i_m + l_m] * i_b[l_m];
}


int main () {
    int size = 32;
    int it = 1;
	std::ofstream outputFile("performance_blocking.csv");
    outputFile << "SIZE,IMPLEMENTATION,GFLOPS\n";

    while (size < 16384) {
        double* l_A = new double[size*size];
        double* l_B = new double[size];
        double* l_C = new double[size];

        uint64_t total_ops = 8ULL*size*size;
        double gflops = 0.0;

        // initialise the Arrays with random values
        for (int i = 0; i < size; ++i) {
            l_B[i] = static_cast<double>(std::rand()) / RAND_MAX;
            l_C[i] = static_cast<double>(std::rand()) / RAND_MAX;
        }
        for (int i = 0; i < size*size; ++i) {
            l_A[i] = static_cast<double>(std::rand()) / RAND_MAX;
        }


        auto l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            impl_blocked_k(l_A, l_B, l_C, size, size, 4);
        }
		auto l_end_time_iteration = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> totalDuration_blocked_k = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_blocked_k.count()));
        outputFile << size << ",blockedk," << gflops <<"\n";

        l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            impl_blocked_m(l_A, l_B, l_C, size, size, 4);
        }
		l_end_time_iteration = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> totalDuration_blocked_m = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_blocked_m.count()));
        outputFile << size << ",blockedm," << gflops <<"\n";  


        l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            matmul_km(l_A, l_B, l_C, size, size);
        }
		l_end_time_iteration = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> totalDuration_kmn = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_kmn.count()));
        outputFile << size << ",non-blocking," << gflops <<"\n"; 
            
        delete[] l_A;
        delete[] l_B;
        delete[] l_C;            
        
        size += size;
    }

    outputFile.close();
    return 0;
}