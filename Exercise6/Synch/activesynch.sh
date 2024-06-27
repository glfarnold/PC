#!/bin/bash
#SBATCH --job-name=activesynch
#SBATCH --output=activesynch.out
#SBATCH --error=activesynch.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=5
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=1

module load mpi

mpicxx activesynch.cpp -o activesynch
mpirun -n 5 ./activesynch
