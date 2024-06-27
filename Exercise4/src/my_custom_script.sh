#!/bin/bash
#SBATCH --job-name=my_custom_reduction
#SBATCH --output=my_custom_reduction.out
#SBATCH --error=my_custom_reduction.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=5
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=1

# Load any necessary modules
module load mpi

# Enter your executable commands here
mpicxx my_custom_reduction.cpp -o my_custom_reduction
mpirun -np 5 ./my_custom_reduction
