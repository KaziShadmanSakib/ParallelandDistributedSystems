CC = gcc
CFLAGS = -Wall -O2
TARGET = serialFloyd

all: serialFloydBuild parallelFloydBuild pipelineFloydBuild

parallelFloydBuild: parallelFloyd.c
	mpicc -o parallelFloyd parallelFloyd.c
parallelFloydRun: parallelFloydBuild
	mpirun --oversubscribe -np 2 ./parallelFloyd


pipelineFloydBuild: pipelineFloyd.c
	mpicc -o pipelineFloyd pipelineFloyd.c
pipelineFloydRun: pipelineFloydBuild
	mpirun --oversubscribe -np 2 ./pipelineFloyd


serialFloydBuild: serialFloyd.c
	$(CC) $(CFLAGS) -o $(TARGET) serialFloyd.c
serialFloydRun: serialFloydBuild
	./serialFloyd

clean:
	rm -f $(TARGET) *.o
