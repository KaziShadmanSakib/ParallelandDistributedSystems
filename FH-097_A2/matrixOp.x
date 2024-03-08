struct Matrix {
    int rows;
    int cols;
    double data<>;
};

struct MatrixPair{
        Matrix matrixA;
        Matrix matrixB;
};

program MATRIXOP_PROG {
    version MATRIXOP_VERS {
        Matrix ADD(MatrixPair) = 1;
        Matrix MULTIPLY(MatrixPair) = 2;
        Matrix INVERSE(Matrix) = 3;
        Matrix TRANSPOSE(Matrix) = 4;
    } = 1;
} = 0x23451111;