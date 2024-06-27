#!/bin/bash
#SBATCH --job-name=monte_carlo
#SBATCH --output=monte_carlo.out
#SBATCH --error=monte_carlo.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=10
#SBATCH --time=00:10:00

module load mpi
module load compiler/gcc/11.2.0

mpicxx monte_carlo.cpp -o monte_carlo
mpirun -n 10 ./monte_carlo

