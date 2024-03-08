#!/bin/bash

# build all the source files 
make

# clear all the output text files
echo "" > Output/serialTime.txt
tr -d '\n' < Output/serialTime.txt > Output/serialTime.txt

echo "" > Output/parallelTime.txt
tr -d '\n' < Output/parallelTime.txt > Output/parallelTime.txt

echo "" > Output/pipelineTime.txt
tr -d '\n' < Output/pipelineTime.txt > Output/pipelineTime.txt


#run serialFloyd MPU Program executable
./serialFloyd

sleep 1

# Run paralleFloyd MPI program executable
MPI_EXECUTABLE="./parallelFloyd"

# Specify the range of processes you want to test
MIN_PROCESSES=2
MAX_PROCESSES=10

# Loop through the range of processes
for ((procs=$MIN_PROCESSES; procs<=$MAX_PROCESSES; procs++))
do
    echo "Running with $procs processes..."
    
    # Use mpirun to execute the MPI program
    mpirun --oversubscribe -np $procs $MPI_EXECUTABLE

    echo "Finished with $procs processes."
    sleep 1
    echo
done

sleep 1

# Run pipelineFloyd MPI program executable
MPI_EXECUTABLE="./pipelineFloyd"

# Specify the range of processes you want to test
MIN_PROCESSES=2
MAX_PROCESSES=10

# Loop through the range of processes
for ((procs=$MIN_PROCESSES; procs<=$MAX_PROCESSES; procs++))
do
    echo "Running with $procs processes..."
    
    # Use mpirun to execute the MPI program
    mpirun --oversubscribe -np $procs $MPI_EXECUTABLE

    echo "Finished with $procs processes."
    sleep 1
    echo
done

sleep 1

python3 plotGraph.py

echo "Script execution complete."
