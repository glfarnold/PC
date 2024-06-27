#!/bin/bash
#SBATCH --job-name=synchronization
#SBATCH --output=synchronization.out
#SBATCH --error=synchronization.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=5
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=1

module load mpi

mpicxx synchronization.cpp -o synchronization
mpirun -n 5 ./synchronization