#!/bin/bash
#SBATCH --job-name=put
#SBATCH --output=put.out
#SBATCH --error=put.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=2
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=1

module load mpi

mpicxx put.cpp -o put
mpirun -n 2 ./put