#include "matrixOp.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>

/* Function for displaying the matrix */
void printMatrix(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%.0lf ", matrix->data.data_val[i * matrix->cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Function for addition of two matrices */
Matrix * add_1_svc(MatrixPair *argp, struct svc_req *rqstp) {
    static Matrix result;

    printf("Addition Operation Initiated in Server..\n");
    
    /* Printing the provided Matrices by the client */
    printf("Provided Matrix: A\n");
    printMatrix(&argp->matrixA);
    printf("\n");
    printf("Provided Matrix: B\n");
    printMatrix(&argp->matrixB);
    printf("\n");

    /* Check for the matrix dimensions */
    if (argp->matrixA.rows != argp->matrixB.rows || argp->matrixA.cols != argp->matrixB.cols) {
        fprintf(stderr, "Matrix dimensions do not match for addition.\n");
        return NULL;  // Handle dimension mismatch error
    }

    /* Allocated memory to store the 2D matrix's data */
    result.rows = argp->matrixA.rows;
    result.cols = argp->matrixA.cols;
    result.data.data_len = result.rows * result.cols;
    result.data.data_val = (double *)malloc(result.data.data_len * sizeof(double));

    if (result.data.data_val == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;  // Handle memory allocation error
    }

    /* Addition of two matrices */
    for (int i = 0; i < result.data.data_len; i++) {
        result.data.data_val[i] = argp->matrixA.data.data_val[i] + argp->matrixB.data.data_val[i];
    }

    /* Printing the result in server */
    printf("Resulting Matrix: C (A + B)\n");
    printMatrix(&result);
    printf("\n");


    return &result;
}

/* Function for multiplication of two matrices */
Matrix * multiply_1_svc(MatrixPair *argp, struct svc_req *rqstp) {
    static Matrix result;

    printf("Multiplication Operation Initiated in Server..\n");
    
    /* Printing the provided Matrices by the client */
    printf("Provided Matrix: A\n");
    printMatrix(&argp->matrixA);
    printf("\n");
    printf("Provided Matrix: B\n");
    printMatrix(&argp->matrixB);
    printf("\n");

    /* Check for the matrix dimensions */
    if (argp->matrixA.cols != argp->matrixB.rows) {
        fprintf(stderr, "Matrix dimensions are not suitable for multiplication.\n");
        return NULL;  // Handle dimension mismatch error
    }

    /* Allocated memory to store the 2D matrix's data */
    result.rows = argp->matrixA.rows;
    result.cols = argp->matrixB.cols;
    result.data.data_len = result.rows * result.cols;
    result.data.data_val = (double *)malloc(result.data.data_len * sizeof(double));

    if (result.data.data_val == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;  // Handle memory allocation error
    }

    /* Multiplication of two matrices */
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < argp->matrixA.cols; k++) {
                sum += argp->matrixA.data.data_val[i * argp->matrixA.cols + k] * 
                       argp->matrixB.data.data_val[k * argp->matrixB.cols + j];
            }
            result.data.data_val[i * result.cols + j] = sum;
        }
    }

    /* Printing the result in server */
    printf("Resulting Matrix: C (A * B)\n");
    printMatrix(&result);
    printf("\n");

    return &result;
}

/* Function for inverse of a matrix */
Matrix * inverse_1_svc(Matrix *argp, struct svc_req *rqstp) {
    static Matrix result;

    printf("Matrix Inversion Operation Initiated in Server..\n");
    
	/* Printing the provided Matrix by the client */
    printf("Provided Matrix:\n");
    printMatrix(argp);
    printf("\n");

    /* Check for the matrix dimensions */
    if (argp->rows != argp->cols) {
        fprintf(stderr, "Matrix must be square for inversion.\n");
        return NULL;  // Handle non-square matrix error
    }

    /* Allocated memory to store the 2D matrix's data */
    gsl_matrix *inputMatrix = gsl_matrix_alloc(argp->rows, argp->cols);


    /* Initializes the inputMatrix with values from the data */
    for (int i = 0; i < argp->rows; i++) {
        for (int j = 0; j < argp->cols; j++) {
            gsl_matrix_set(inputMatrix, i, j, argp->data.data_val[i * argp->cols + j]);
        }
    }

    /* Allocate memory for a permutation object */
    gsl_permutation *p = gsl_permutation_alloc(argp->rows);
    int signum;

    // Perform LU decomposition
    if (gsl_linalg_LU_decomp(inputMatrix, p, &signum) != GSL_SUCCESS) {
        fprintf(stderr, "Matrix is not invertible.\n");
        gsl_matrix_free(inputMatrix);
        gsl_permutation_free(p);
        return NULL;  // Handle non-invertible matrix error
    }

    /* Allocate memory for a matrix that will likely store the inverse of inputMatrix  */
    gsl_matrix *invMatrix = gsl_matrix_alloc(argp->rows, argp->cols);
    
    // Perform LU inversion
    if (gsl_linalg_LU_invert(inputMatrix, p, invMatrix) != GSL_SUCCESS) {
        fprintf(stderr, "Matrix is not invertible.\n");
        gsl_matrix_free(inputMatrix);
        gsl_permutation_free(p);
        gsl_matrix_free(invMatrix);
        return NULL;  // Handle non-invertible matrix error
    }

    /* Allocated memory to store the 2D matrix's data */
    result.rows = argp->rows;
    result.cols = argp->cols;
    result.data.data_len = result.rows * result.cols;
    result.data.data_val = (double *)malloc(result.data.data_len * sizeof(double));

    if (result.data.data_val == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        gsl_matrix_free(inputMatrix);
        gsl_permutation_free(p);
        gsl_matrix_free(invMatrix);
        return NULL;  // Handle memory allocation error
    }

    /* Copying the inverse matrix data into the result */
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data.data_val[i * result.cols + j] = gsl_matrix_get(invMatrix, i, j);
        }
    }

    /* Printing the result in server */
    printf("Inverse Matrix:\n");
    printMatrix(&result);
    printf("\n");

    // Free GSL matrices and permutation
    gsl_matrix_free(inputMatrix);
    gsl_permutation_free(p);
    gsl_matrix_free(invMatrix);

    return &result;
}

/* Function for transpose of a matrix */
Matrix * transpose_1_svc(Matrix *argp, struct svc_req *rqstp) {
    static Matrix result;

    printf("Transpose Operation Initiated in Server..\n");
    
    /* Printing the provided Matrix by the client */
    printf("Provided Matrix:\n");
    printMatrix(argp);
    printf("\n");

    /* Allocated memory to store the 2D matrix's data */
    result.rows = argp->cols; // Swap rows and columns for the transpose
    result.cols = argp->rows;
    result.data.data_len = result.rows * result.cols;
    result.data.data_val = (double *)malloc(result.data.data_len * sizeof(double));

    if (result.data.data_val == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;  // Handle memory allocation error
    }

    /* Transpose of a matrix */
    for (int i = 0; i < argp->rows; i++) {
        for (int j = 0; j < argp->cols; j++) {
            result.data.data_val[j * result.cols + i] = argp->data.data_val[i * argp->cols + j];
        }
    }

    /* Printing the result in server */
    printf("Transposed Matrix:\n");
    printMatrix(&result);
    printf("\n");

    return &result;
}
