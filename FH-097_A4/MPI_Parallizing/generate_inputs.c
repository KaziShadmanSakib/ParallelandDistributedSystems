#include <stdio.h>

#define INF 99999

void generateInput(int n) {
    // Open a file for writing
    FILE *file = fopen("sample_inputs.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Print the header with the number of nodes to the file
    fprintf(file, "%d\n", n);

    int d = 3;
    int a = 1;
    int flagd = 0;
    // Generate the pattern for the input matrix and print to the file
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                fprintf(file, "0 ");
            } 
            else if(j == i + 1){
                if(!flagd){
                    fprintf(file, "%d ",d);
                    flagd = 1;
                }
                else{
                    fprintf(file, "%d ", (i + j) * 2);
                }
            }
            else {
                fprintf(file, "%d ", INF);
            }
        }
        fprintf(file, "\n");
    }

    // Close the file
    fclose(file);

    printf("File 'sample_inputs.txt' created successfully.\n");
}

int main() {
    int n;

    while(1){
        // Get the value of n
        printf("Enter the number of nodes (n): ");
        scanf("%d", &n);

        generateInput(n);
    }

    return 0;
}
