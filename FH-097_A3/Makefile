CC = gcc
MPICC = mpicc
CFLAGS = -w -O2

all: sequentialPI parallelPI dynamicSpawnPI worker

sequentialPI: sequentialPI.c
	$(CC) $(CFLAGS) -o sequentialPI sequentialPI.c

parallelPI: parallelPI.c
	$(MPICC) $(CFLAGS) -o parallelPI parallelPI.c

dynamicSpawnPI: dynamicSpawnPI.c
	$(MPICC) $(CFLAGS) -o dynamicSpawnPI dynamicSpawnPI.c

worker: worker.c
	$(MPICC) $(CFLAGS) -o worker worker.c

clean:
	rm -f sequentialPI parallelPI dynamicSpawnPI worker

.PHONY: run_sequential run_parallel_2 run_parallel_4 run_parallel_6 run_parallel_8 run_parallel_16 run_parallel_32 run_parallel_64 run_dynamic_2 run_dynamic_4 run_dynamic_6 run_dynamic_8 run_dynamic_16 run_dynamic_32 run_dynamic_64 run_all

run_sequential: sequentialPI
	./sequentialPI

run_parallel_2: parallelPI
	mpirun --oversubscribe -np 2 ./parallelPI

run_parallel_4: parallelPI
	mpirun --oversubscribe -np 4 ./parallelPI

run_parallel_6: parallelPI
	mpirun --oversubscribe -np 6 ./parallelPI

run_parallel_8: parallelPI
	mpirun --oversubscribe -np 8 ./parallelPI

run_parallel_16: parallelPI
	mpirun --oversubscribe -np 16 ./parallelPI

run_parallel_32: parallelPI
	mpirun --oversubscribe -np 32 ./parallelPI

run_parallel_64: parallelPI
	mpirun --oversubscribe -np 64 ./parallelPI

run_dynamic_2: dynamicSpawnPI
	mpirun --oversubscribe -np 2 ./dynamicSpawnPI

run_dynamic_4: dynamicSpawnPI
	mpirun --oversubscribe -np 4 ./dynamicSpawnPI

run_dynamic_6: dynamicSpawnPI
	mpirun --oversubscribe -np 6 ./dynamicSpawnPI

run_dynamic_8: dynamicSpawnPI
	mpirun --oversubscribe -np 8 ./dynamicSpawnPI

run_dynamic_16: dynamicSpawnPI
	mpirun --oversubscribe -np 16 ./dynamicSpawnPI

run_dynamic_32: dynamicSpawnPI
	mpirun --oversubscribe -np 32 ./dynamicSpawnPI

run_dynamic_64: dynamicSpawnPI
	mpirun --oversubscribe -np 64 ./dynamicSpawnPI

run_speedup_plotter:
	python3 speedup_plotter.py

run_all: run_sequential run_parallel_2 run_parallel_4 run_parallel_6 run_parallel_8 run_parallel_16 run_parallel_32 run_parallel_64 run_dynamic_2 run_dynamic_4 run_dynamic_6 run_dynamic_8 run_dynamic_16 run_dynamic_32 run_dynamic_64 run_speedup_plotter