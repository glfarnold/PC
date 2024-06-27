#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <math.h>

void matmul_mnk(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k ) {

  for( std::size_t l_m = 0; l_m < i_m; l_m++ )
    for( std::size_t l_n = 0; l_n < i_n; l_n++ )
      for( std::size_t l_k = 0; l_k < i_k; l_k++ )
        io_C[l_m*i_n + l_n] += i_A[l_m*i_k + l_k] * i_B[l_k*i_n + l_n];
}

void matmul_nmk(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k ) {

    for( std::size_t l_n = 0; l_n < i_n; l_n++ )
        for( std::size_t l_m = 0; l_m < i_m; l_m++ )
            for( std::size_t l_k = 0; l_k < i_k; l_k++ )
                io_C[l_m*i_n + l_n] += i_A[l_m*i_k + l_k] * i_B[l_k*i_n + l_n];
}

void matmul_mkn(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k ) {

    for( std::size_t l_m = 0; l_m < i_m; l_m++ )
        for( std::size_t l_k = 0; l_k < i_k; l_k++ )
            for( std::size_t l_n = 0; l_n < i_n; l_n++ )
                io_C[l_m*i_n + l_n] += i_A[l_m*i_k + l_k] * i_B[l_k*i_n + l_n];
}

void matmul_nkm(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k ) {

    for( std::size_t l_n = 0; l_n < i_n; l_n++ )
        for( std::size_t l_k = 0; l_k < i_k; l_k++ )
            for( std::size_t l_m = 0; l_m < i_m; l_m++ )
                io_C[l_m*i_n + l_n] += i_A[l_m*i_k + l_k] * i_B[l_k*i_n + l_n];
}

void matmul_knm(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k ) {

    for( std::size_t l_k = 0; l_k < i_k; l_k++ )
        for( std::size_t l_n = 0; l_n < i_n; l_n++ )
            for( std::size_t l_m = 0; l_m < i_m; l_m++ )
                io_C[l_m*i_n + l_n] += i_A[l_m*i_k + l_k] * i_B[l_k*i_n + l_n];
}

void matmul_kmn(double* i_A,
                double* i_B,
                double* io_C,
                std::size_t i_m,
                std::size_t i_n,
                std::size_t i_k ) {

    for( std::size_t l_k = 0; l_k < i_k; l_k++ )
        for( std::size_t l_m = 0; l_m < i_m; l_m++ )
            for( std::size_t l_n = 0; l_n < i_n; l_n++ )
                io_C[l_m*i_n + l_n] += i_A[l_m*i_k + l_k] * i_B[l_k*i_n + l_n];
}

int main () {
    int size = 2;
    int it = 10;
	std::ofstream outputFile("performance.csv");
    outputFile << "M=N=K,IMPLEMENTATION,GFLOPS\n";

    while (size < 2048) {
        double* l_A = new double[size*size];
        double* l_B = new double[size*size];
        double* l_C = new double[size*size];

        uint64_t total_ops = 2ULL*size*size*size;
        double gflops = 0.0;

        // initialise the Arrays with random values
        for (int i = 0; i < size; ++i) {
            l_A[i] = static_cast<double>(std::rand()) / RAND_MAX;
            l_B[i] = static_cast<double>(std::rand()) / RAND_MAX;
            l_C[i] = static_cast<double>(std::rand()) / RAND_MAX;
        }


        
        auto l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            matmul_mnk(l_A, l_B, l_C, size, size, size);
        }
		auto l_end_time_iteration = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> totalDuration_mnk = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_mnk.count()));
        outputFile << size << ",mnk," << gflops <<"\n"; 

        l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            matmul_mkn(l_B, l_A, l_C, size, size, size);
        }
		l_end_time_iteration = std::chrono::high_resolution_clock::now(); 

        std::chrono::duration<double> totalDuration_mkn = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_mkn.count()));
        outputFile << size << ",mkn," << gflops<<"\n"; 

        
        l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            matmul_nmk(l_A, l_B, l_C, size, size, size);
        }
		l_end_time_iteration = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> totalDuration_nmk = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_nmk.count()));
        outputFile << size << ",nmk," << gflops <<"\n";  



        l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            matmul_nkm(l_A, l_B, l_C, size, size, size);        
        }     
		l_end_time_iteration = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> totalDuration_nkm = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_nkm.count()));
        outputFile << size << ",nkm," << gflops <<"\n"; 



        l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            matmul_kmn(l_A, l_B, l_C, size, size, size);
        }
		l_end_time_iteration = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> totalDuration_kmn = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_kmn.count()));
        outputFile << size << ",kmn," << gflops <<"\n"; 



        l_start_time_iteration = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < it; i++) {
            matmul_knm(l_A, l_B, l_C, size, size, size);
        }
		l_end_time_iteration = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> totalDuration_knm = (l_end_time_iteration - l_start_time_iteration) / it;
        gflops = total_ops / (pow(10,9) * (totalDuration_knm.count()));
        outputFile << size << ",knm," << gflops <<"\n"; 
			
            
        delete[] l_A;
        delete[] l_B;
        delete[] l_C;            
        
        size += size;
    }

    outputFile.close();
    return 0;
}