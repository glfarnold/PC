#!/bin/bash
#SBATCH --job-name=both
#SBATCH --output=both.out
#SBATCH --error=both.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=2
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=1

module load mpi

mpicxx both.cpp -o both
mpirun -n 2 ./both