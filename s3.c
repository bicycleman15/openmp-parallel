#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <omp.h>

void crout(double **A, double **L, double **U, int n) {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		U[i][i] = 1;
	}

    
	for (int j = 0; j < n; j++) {
        
        /* do this so that L[j][j] can be used parallely */
        double temp = 0;
        for (int k = 0; k < j; k++) {
            temp = temp + L[j][k] * U[k][j];
        }
        L[j][j] = A[j][j] - temp;

        /* Check early on */
        if(L[j][j] == 0) {
            printf("Exiting program !!!\n");		
            exit(0);
        }

        #pragma omp parallel sections
        {   
            #pragma omp section
            {   
                #pragma omp parallel
                for (int i = j+1; i < n; i++) {
                    double sum = 0;
                    for (int k = 0; k < j; k++) {
                        sum = sum + L[i][k] * U[k][j];	 // [0,3] [1,3] [2,3] 
                    }
                    L[i][j] = A[i][j] - sum;
                }
            }
            
            #pragma omp section
            {   
                #pragma omp parallel
                for (int i = j; i < n; i++) {
                    double sum = 0;
                    for(int k = 0; k < j; k++) {
                        sum = sum + L[j][k] * U[k][i];  // [0:2 , 3] [0:2, 4]
                    }
                    U[j][i] = (A[j][i] - sum) / L[j][j];  // [3, 3] [3, 4]
                }
            }
        }
	}
}

void write_output(char fname[], double** arr, int n ){
	FILE *f = fopen(fname, "w");
	for( int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			fprintf(f, "%0.12f ", arr[i][j]);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

void print_matrix(double **A, int N){
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            printf("%g ", A[i][j]);
        }
        printf("\n");
    }
}

int main(){
    // TODO : Optimize input
    omp_set_num_threads(4);
    int N = 4; char fname[] = "input.txt";
    double **A,**L,**U;
    A = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) A[i] = malloc(N*sizeof(double));
    L = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) L[i] = malloc(N*sizeof(double));
    U = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) U[i] = malloc(N*sizeof(double));
    FILE *f = fopen(fname, "r");
    char str[20];
    for(int i = 0; i < N*N; ++i){
        fscanf(f,"%s", str);
        A[i/N][i%N] = atof(str);
    }
    fclose(f);

	// print_matrix(A, N); printf("\n");
    
    // Sequential
    crout(A,L,U,N);

	// print_matrix(L, N); printf("\n");
	// print_matrix(U, N); printf("\n");
    write_output("output_L.txt", L, N);
    write_output("output_U.txt", U, N);

    return 0;
}