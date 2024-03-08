/* worker.c */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double dboard(int darts);
void initializeRandom(unsigned seed);

int main(int argc, char *argv[]) {
    int rounds = 100;  // Number of rounds for this worker
    int darts = 5000000; // Number of darts to throw for each round
    int size;
    int taskId, numTasks;
    MPI_Comm parent;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks); // Get the total number of tasks
    MPI_Comm_rank(MPI_COMM_WORLD, &taskId);   // Get the task's ID

    MPI_Comm_get_parent(&parent);
    if (parent == MPI_COMM_NULL)
        perror("No parent!");

    MPI_Comm_remote_size(parent, &size);
    if (size != 1)
        perror("Something's wrong with the parent");

    initializeRandom(taskId); // Initialize random seed based on the task's ID

    /*To distribute the workload evenly among the worker processes*/
    darts = (darts/(numTasks-1));
    
    double score = 0;
    for (int i = 0; i < rounds; i++) {
        /* All tasks calculate pi using dartboard algorithm */
        double homepi = dboard(darts);
        printf("Pi value for %d round: %lf\n", i, homepi);
        score += homepi;
    }

    // Calculate the average Pi estimate for this worker
    double pi_worker = (double)score / (double)(rounds);

    // Send the average Pi estimate back to the master
    MPI_Send(&pi_worker, 1, MPI_DOUBLE, 0, 0, parent);

    MPI_Finalize();
    return 0;
}

// Function to initialize random number generator
void initializeRandom(unsigned seed) {
    // unsigned seed = (unsigned)time(NULL); // Use current time as the seed
    srandom(seed);
}

double dboard(int darts) {
    int score = 0;
    double x_coord, y_coord, pi, r;

    for (int n = 1; n <= darts; n++){
        r = (double)rand() / RAND_MAX; // Generate random numbers for x and y coordinates
        x_coord = (2.0 * r) - 1.0;

        r = (double)rand() / RAND_MAX;
        y_coord = (2.0 * r) - 1.0;

        // If dart lands in the circle, increment the score
        if ((x_coord * x_coord + y_coord * y_coord) <= 1.0)
            score++;
    }

    // Calculate pi
    pi = 4.0 * (double)score / (double)darts;
    return pi;
}
