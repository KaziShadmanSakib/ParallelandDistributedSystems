#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define INF 99999
#define MAX_NODES 500

int graph[MAX_NODES][MAX_NODES];

void floyd_pipeline(const char* inputFileName, const char* outputFileName, int graphSize);


void floyd_pipeline(const char* inputFileName, const char* outputFileName, int graphSize) {
    
    int n = graphSize;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Only Master process (rank 0) reads the input from a file
    if (rank == 0) {
        FILE* input_file = fopen(inputFileName, "r");
        if (input_file == NULL) {
            fprintf(stderr, "Error opening input file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            exit(1);
        }

        fscanf(input_file, "%d", &n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (fscanf(input_file, "%d", &graph[i][j]) != 1) {
                    fprintf(stderr, "Error reading from input file.\n");
                    MPI_Abort(MPI_COMM_WORLD, 1);
                    exit(1);
                }
            }
        }

        fclose(input_file);
    }

    // Record the start time
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int k = 0; k < n; k++) {
        // Broadcast the value of n to all processes
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Broadcast the entire matrix to all processes
        MPI_Bcast(graph, n * n, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = rank; i < n; i += size) {
            for (int j = 0; j < n; j++) {
                if (graph[i][k] + graph[k][j] < graph[i][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                }
            }
        }
    }


    // Record the end time
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();


    printf("Process: %d takes execution time: %f\n", rank, end_time-start_time);

    // Only Master process (rank 0) writes the output to a file
    if (rank == 0) {
        FILE* output_file = fopen(outputFileName, "w");
        if (output_file == NULL) {
            fprintf(stderr, "Error opening output file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            exit(1);
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(output_file, "%d ", graph[i][j]);
            }
            fprintf(output_file, "\n");
        }

        fclose(output_file);

        double cpu_time_used = end_time - start_time;
        printf("MPI Execution Time for n = %d: %f seconds\n", n, cpu_time_used);

        // write execution time to exe_time.txt file
        FILE *exeTimeFile;
        char exeFilename[] = "Output/pipelineTime.txt";
        exeTimeFile = fopen(exeFilename, "a");      // append
        if (exeTimeFile == NULL) {
            fprintf(stderr, "Could not open file %s\n", exeFilename);
            exit(1);
        }
        fprintf(exeTimeFile, "%d ", size);
        fprintf(exeTimeFile, "%lf\n", cpu_time_used);
        fclose(exeTimeFile);
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int n = 4;

    floyd_pipeline("input.txt", "output.txt", n);

    MPI_Finalize();

    return 0;
}


