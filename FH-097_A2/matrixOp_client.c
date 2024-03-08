#include "matrixOp.h"
#include <stdio.h>
#include <stdlib.h>

struct InputMatrices {
    Matrix matrix[2];
};
typedef struct InputMatrices InputMatrices;

/* Function for reading input from the file */
void readInput(FILE *file, Matrix *matrix) {
	fscanf(file, "%d", &matrix->rows);
  	fscanf(file, "%d", &matrix->cols);
  	
  	matrix->data.data_val = (double*)malloc(matrix->rows * matrix->cols * sizeof(double));
  	matrix->data.data_len = matrix->rows * matrix->cols;
  	
  	for (int i = 0; i < matrix->rows; i++) {
    	for (int j = 0; j < matrix->cols; j++) {
      	fscanf(file, "%lf", &matrix->data.data_val[i * matrix->cols + j]);
    	}
  	}
}

/* Function for displaying the matrix */
void printMatrix(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%.2lf ", matrix->data.data_val[i * matrix->cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Function which communicates with the server for results */
void matrixop_prog_1(char *host, int operation, InputMatrices inputMatrices) {
	CLIENT *clnt;
	Matrix *result;
	MatrixPair matrixPair;
	Matrix matrix;

#ifndef	DEBUG
	clnt = clnt_create (host, MATRIXOP_PROG, MATRIXOP_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	switch (operation) {

			/* Matrix Addition */
		    case 1:
		    	
		    	matrixPair.matrixA = inputMatrices.matrix[0];
		    	matrixPair.matrixB = inputMatrices.matrix[1];
		    	
		    	/* Check for the matrix dimensions */
		    	if(matrixPair.matrixA.rows != matrixPair.matrixB.rows
		    		|| matrixPair.matrixA.cols != matrixPair.matrixB.cols){
		    		printf("***Choose matrices of the same dimension please!***\n\n\n");
		    		break;
		    	}

		    	/* Communicate with the server to get matrix addition result */
			    else{
			    	result = add_1(&matrixPair, clnt);
					if (result == (Matrix *) NULL) {
						clnt_perror (clnt, "call failed");
					}
			    }

			    /* Display the result Matrix */
			    printf("The sum of two matrices is:\n");
				printMatrix(result);

		        break;

		    /* Matrix Multiplication */
		    case 2:

		    	matrixPair.matrixA = inputMatrices.matrix[0];
		    	matrixPair.matrixB = inputMatrices.matrix[1];

		    	/* Check for the matrix dimensions */
			    if(matrixPair.matrixA.cols != matrixPair.matrixB.rows){
		    		printf("***Matrix dimensions are not suitable for multiplication.***\n\n\n");
		    		break;
		    	}

		    	/* Communicate with the server to get matrix multiplication result */
			    else{
			    	result = multiply_1(&matrixPair, clnt);
					if (result == (Matrix *) NULL) {
						clnt_perror (clnt, "call failed");
					}
			    }

			    /* Display the result Matrix */
				printf("The multiplication of two matrices is:\n");
				printMatrix(result);

		        break;
		    
		    /* Matrix Inversion */
			case 3:

				matrix = inputMatrices.matrix[0];

				/* Communicate with the server to get inverted matrix result */
				result = inverse_1(&matrix, clnt);
				if (result == (Matrix *) NULL) {
				    clnt_perror(clnt, "call failed");
				    printf("The matrix is not invertible.\n\n");
				} else {

					/* Display the result Matrix */
				    printf("The inverse of the matrix is:\n");
				    printMatrix(result);
				}

				break;
			
			/* Transpose Matrix */
			case 4:

				matrix = inputMatrices.matrix[0];
			  	
			  	/* Communicate with the server to get transpose matrix result */
			    result = transpose_1(&matrix, clnt);
				if (result == (Matrix *) NULL) {
					clnt_perror (clnt, "call failed");
				}

				/* Display the result Matrix */
				printf("The transpose of the matrix is:\n");
				printMatrix(result);

		        break;

		}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int main (int argc, char *argv[]){
	char *host;
	int operation;
	InputMatrices inputMatrices;
	char file_name[256]; // Assuming the file name won't exceed 255 characters


	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}

	host = argv[1];

	printf("Welcome to the Matrix...\n");
		
	printf("Enter the name of the input file: ");
	
	/* User input of the input file name */
	scanf("%255s", file_name); // Limit the input to 255 characters
	FILE *inputFile = fopen(file_name, "r");
	if (inputFile == NULL) {
	    perror("Failed to open input file");
	    exit(1);
	}
	printf("\n");

	while(1){
		
		printf("Choose your operation\n1. Matrix Addition\n2. Matrix Multiplication\n3. Matrix Inverse\n4. Matrix Transpose\nEnter 0 to QUIT!\n\n");
		printf("Enter Operation: ");
		scanf("%d", &operation);
		printf("\n");
		if(operation == 0){
			break;
		}
		
		switch (operation) {

			/* Matrix Addition */
		    case 1:
		    	inputFile = fopen(file_name, "r");
	            if (inputFile == NULL) {
	                perror("Failed to open input file");
	                exit(1);
	            }

	            /* Reads the input matrices from the file */
			    readInput(inputFile, &inputMatrices.matrix[0]);
			    readInput(inputFile, &inputMatrices.matrix[1]);
			    fclose(inputFile);

		        break;
		    
		    /* Matrix Multiplication */
		    case 2:
			
				inputFile = fopen(file_name, "r");
	            if (inputFile == NULL) {
	                perror("Failed to open input file");
	                exit(1);
	            }

	            /* Reads the input matrices from the file */
			    readInput(inputFile, &inputMatrices.matrix[0]);
			    readInput(inputFile, &inputMatrices.matrix[1]);
			    fclose(inputFile);
		        		        
		        break;
		    
		    /* Matrix Inversion */
			case 3:

				inputFile = fopen(file_name, "r");
	            if (inputFile == NULL) {
	                perror("Failed to open input file");
	                exit(1);
	            }

	            /* Reads the input matrix from the file */
			    readInput(inputFile, &inputMatrices.matrix[0]);
			    fclose(inputFile);
			    
			    break;

			/* Transpose Matrix */
			case 4:

				inputFile = fopen(file_name, "r");
	            if (inputFile == NULL) {
	                perror("Failed to open input file");
	                exit(1);
	            }

	            /* Reads the input matrix from the file */
			    readInput(inputFile, &inputMatrices.matrix[0]);
				fclose(inputFile);
			    
			    break;

		    /* Error msg for invalid selection of operation number */
		    default:
		        printf("Invalid selection of operation number! Try Again..!\n\n");
		}

		matrixop_prog_1 (host, operation, inputMatrices);

	}

	exit (0);
}

