#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>

int main() {

	const int arraySize = 36;
	int l_size[arraySize];
	int size = 512;
	for (int i = 0; i < arraySize / 2; ++i) {
		l_size[2*i] = size;
		l_size[2*i+1] = size + size/2;
		size *= 2;
	}
  

	double l_scalar = 2.0;
	
	std::ofstream outputFile("mem_bandwidth.csv");
	outputFile << "Array Size (bytes),Bandwidth (GB/s)\n";

	for(int i = 0; i < sizeof(l_size) / sizeof(l_size[0]); ++i){  

		int l_s = l_size[i];
		std::chrono::duration<double> totalDuration;

		int iterations = 10000;
		for(int j = 0; j < iterations; ++j){
		
			double* l_A = new double[l_s];
			double* l_B = new double[l_s];
			double* l_C = new double[l_s];
		
			for (int i = 0; i < l_s; ++i) {
				l_A[i] = static_cast<double>(std::rand()) / RAND_MAX;
				l_B[i] = static_cast<double>(std::rand()) / RAND_MAX;
				l_C[i] = 0.0;
			}
		
			auto l_start_time_iteration = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < l_s; ++i) {
				l_C[i] = l_A[i] + l_scalar * l_B[i];
			}
			auto l_end_time_iteration = std::chrono::high_resolution_clock::now();
		
			totalDuration += l_end_time_iteration - l_start_time_iteration;
      
			delete[] l_A;
			delete[] l_B;
			delete[] l_C;
		  }
		double l_data_access_speed = 3.0 * l_s * sizeof(double) * iterations / totalDuration.count() / (1024 * 1024 * 1024);
		std::cout << "STREAM Benchmark: " << l_data_access_speed << " GB/s at " << l_s << " elements" << std::endl;

		outputFile << l_s << "," << l_data_access_speed << "\n";
	}
	return 0;
}
