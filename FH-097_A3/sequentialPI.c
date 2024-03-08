/***************************************************************************
 * FILE: sequentialPI.c
 * DESCRIPTION:  
 *   Serial pi Calculation - C Version
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  
 * AUTHOR: unknown
 * REVISED: 10/05/23 Kazi Shadman Sakib
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void srandom (unsigned seed);  
double dboard (int darts);

#define DARTS 5000000   	/* number of throws at dartboard */
#define ROUNDS 100    	/* number of times "darts" is iterated */

int main(int argc, char *argv[]){
   double pi;          	/* average of pi after "darts" is thrown */
   double avepi;       	/* average pi value for all iterations */
   int i, n;

   /* Initialize start time, end time and cpu time used */
   clock_t start_time, end_time;
   double cpu_time_used;


   // Open the output file in write mode
   FILE *output_file = fopen("output.txt", "w");
   if (output_file == NULL) {
      fprintf(stderr, "Could not open the output file.\n");
      exit(EXIT_FAILURE);
   }

   printf("Starting serial version of pi calculation using dartboard algorithm...\n");
   srandom (5);            /* seed the random number generator */
   avepi = 0;

   /* Record the start time */
   start_time = clock();

   for (i = 0; i < ROUNDS; i++) {
      /* Perform pi calculation on serial processor */
      pi = dboard(DARTS);
      avepi = ((avepi * i) + pi)/(i + 1); 
      printf("   After %3d throws, average value of pi = %10.8f\n",
            (DARTS * (i + 1)),avepi);
   }

   /* Record the end time */
   end_time = clock();

   /* Calculating the CPU time needed to calculate PI */
   cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
   printf("\nTime taken to calculate pi: %lf seconds\n", cpu_time_used);

   // Save cpu_time_used to the output file
   fprintf(output_file, "%lf\n", cpu_time_used);

   printf("Real value of PI: %10.8f \n\n", avepi);
   
   // Close the output file
   fclose(output_file);

   return 0;
}


/*****************************************************************************
 * dboard
 *****************************************************************************/
#define sqr(x)	((x)*(x))
long random(void);

double dboard(int darts){
   double x_coord,       /* x coordinate, between -1 and 1  */
          y_coord,       /* y coordinate, between -1 and 1  */
          pi,            /* pi  */
          r;             /* random number scaled between 0 and 1  */
   int score,            /* number of darts that hit circle */
       n;
   unsigned int cconst;  /* must be 4-bytes in size */
/*************************************************************************
 * The cconst variable must be 4 bytes. We check this and bail if it is
 * not the right size
 ************************************************************************/
   if (sizeof(cconst) != 4) {
      printf("Wrong data size for cconst variable in dboard routine!\n");
      printf("See comments in source file. Quitting.\n");
      exit(1);
      }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

/***********************************************************************
 * Throw darts at board.  Done by generating random numbers
 * between 0 and 1 and converting them to values for x and y
 * coordinates and then testing to see if they "land" in
 * the circle."  If so, score is incremented.  After throwing the
 * specified number of darts, pi is calculated.  The computed value
 * of pi is returned as the value of this function, dboard.
 ************************************************************************/

   for (n = 1; n <= darts; n++) {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
         score++;
   }

   /* calculate pi */
   pi = 4.0 * (double)score/(double)darts;
   return(pi);
} 