#!/bin/bash
#SBATCH --job-name=benchmark
#SBATCH --output=benchmark.out
#SBATCH --error=benchmark.err
#SBATCH --partition=s_hadoop
#SBATCH -N 1
#SBATCH --cpus-per-task=72
#SBATCH -n 1

module load tools/python/3.8
module load compiler/gcc/9.3.0

# Compile the program
g++ measure_memory.cpp -fopenmp -Ofast -ffast-math -march=native -o measure_memory

# Execute the compiled program
./measure_memory
python visualisation.py


