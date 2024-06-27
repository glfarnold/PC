#!/bin/bash
#SBATCH --job-name=tree_b
#SBATCH --output=tree_b.out
#SBATCH --error=tree_b.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=72
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=1

module load mpi

mpicxx tree_broadcast.cpp -o tree_b
mpirun -n 72 ./tree_b 100