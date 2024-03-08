#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INF 99999

void floyd_serial(int** graph, int n);

void floyd_serial(int** graph, int n) {
    
    // Diagonals should be set to 0 as the distance from a node to itself is always 0.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // If i == j, set distance to 0; otherwise, use the graph's edge weight
            if (i == j) {
                graph[i][j] = 0;
            }
        }
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (graph[i][k] != INF && graph[k][j] != INF &&
                    graph[i][k] + graph[k][j] < graph[i][j]) {
                    // Update the shortest distance if a shorter path is found through an intermmediate node
                    graph[i][j] = graph[i][k] + graph[k][j];
                }
            }
        }
    }
}

int main() {

    FILE *inputFile, *outputFile;
    char inputFilename[] = "input.txt";
    char outputFilename[] = "output.txt";
    
    inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Could not open file %s\n", inputFilename);
        return 1;
    }

    int n;

    fscanf(inputFile, "%d", &n);

    // Allocate memory for the graph
    int** graph = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        graph[i] = (int*)malloc(n * sizeof(int));

    // Read the graph input
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(inputFile, "%d", &graph[i][j]);
        }
    }

    fclose(inputFile);

    clock_t start, end;
    double cpu_time_used;
    
    start = clock();

    floyd_serial(graph, n);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("CPU Execution Time for n = %d: %f seconds\n", n, cpu_time_used);

    // Open the output file
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Could not open file %s\n", outputFilename);
        return 1;
    }

    // Write the result to the output file
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(outputFile, "%d ", graph[i][j]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);

    // Free allocated memory
    for (int i = 0; i < n; i++)
        free(graph[i]);
    free(graph);

    // write execution time in exe_time.txt file
    FILE *exeTimeFile;
    char exeFilename[] = "Output/serialTime.txt";
    exeTimeFile = fopen(exeFilename, "w");
    if (exeTimeFile == NULL) {
        fprintf(stderr, "Could not open file %s\n", exeFilename);
        exit(1);
    }
    fprintf(exeTimeFile, "%lf\n", cpu_time_used);
    fclose(exeTimeFile);

    return 0;
}
