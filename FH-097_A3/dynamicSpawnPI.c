/* dynamicSpawnPI.c */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int taskId, numTasks, i;
    MPI_Status status;
    MPI_Comm intercomm;
    char worker_program[50] = "./worker"; // Path to the worker executable
    /* Initialize start time, end time and cpu time used */
    clock_t start_time, end_time;
    double cpu_time_used;

    // Open the output file in append mode
    FILE *output_file = fopen("output.txt", "a");
    if (output_file == NULL) {
        fprintf(stderr, "Could not open the output file.\n");
        exit(EXIT_FAILURE);
    }

    /* Record the start time */
    start_time = clock();

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks); // Get the total number of tasks
    MPI_Comm_rank(MPI_COMM_WORLD, &taskId);   // Get the task's ID

    if (taskId == 0) {
        /* Dynamic spawing of workers */
        MPI_Comm_spawn(worker_program, MPI_ARGV_NULL, numTasks, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);

        double pi_total = 0.0;

        for (i = 0; i < numTasks; i++) {
            double pi_worker;
            clock_t process_start_time, process_end_time;
            process_start_time = clock();
            // Receive Pi estimate from each worker
            MPI_Recv(&pi_worker, 1, MPI_DOUBLE, i, 0, intercomm, &status);
            pi_total += pi_worker;
            /* Record the end time */
            process_end_time = clock();

            /* Calculating the CPU time needed to calculate PI */
            double process_time_used = ((double) (process_end_time - process_start_time)) / CLOCKS_PER_SEC;

            printf("time taken for process %d is: %lf\n", i+1, process_time_used);
        }



        // Compute the overall Pi estimate
        double pi_avg = pi_total / numTasks;
        
        /* Record the end time */
        end_time = clock();

        /* Calculating the CPU time needed to calculate PI */
        cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
        printf("\nTime taken to calculate pi: %lf seconds\n", cpu_time_used);

        // Save cpu_time_used to the output file
        fprintf(output_file, "%lf\n", cpu_time_used);
          
        // Close the output file
        fclose(output_file);

        printf("Average Pi estimate: %lf\n\n", pi_avg);
    }

    MPI_Finalize();
    return 0;
}
