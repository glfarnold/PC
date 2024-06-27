#!/bin/bash
#SBATCH --job-name=fibonacci
#SBATCH --output=fibonacci.out
#SBATCH --error=fibonacci.err
#SBATCH --partition=s_hadoop
#SBATCH -N 1
#SBATCH --cpus-per-task=72
#SBATCH -n 1
#SBATCH --time=00:10:00

# Record the job start time
start_time=$(date +%s)

# Compile the program
g++ -o fibonacci fibonacci.cpp

# Execute the compiled program
echo "100" | ./fibonacci

# Record the job end time
end_time=$(date +%s)

# Calculate and print consumed time
consumed_time=$((end_time - start_time))
echo "consumed time (in seconds): $consumed_time" 
