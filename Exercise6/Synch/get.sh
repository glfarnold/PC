#!/bin/bash
#SBATCH --job-name=get
#SBATCH --output=get.out
#SBATCH --error=get.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=2
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=1

module load mpi

mpicxx get.cpp -o get
mpirun -n 2 ./get