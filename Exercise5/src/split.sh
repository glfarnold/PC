#!/bin/bash
#SBATCH --job-name=split
#SBATCH --output=split.out
#SBATCH --error=split.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --time=00:05:00
#SBATCH --cpus-per-task=1

module load mpi

mpicxx split.cpp -o split_c
mpirun -n 16 ./split_c
