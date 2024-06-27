#!/bin/bash
#SBATCH --output=matrix.out
#SBATCH --error=matrix.err
#SBATCH --partition=s_hadoop
#SBATCH --nodes=1
#SBATCH --ntasks=72
#SBATCH --time=00:10:00
#SBATCH --cpus-per-task=1

module load tools/python/3.8

g++ -o main main.cpp -march=native -O2 -std=c++11
./main 
python plot.py