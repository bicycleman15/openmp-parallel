#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

void crout(double **A, double **L, double **U, int n) {
	for (int i = 0; i < n; i++) {
		U[i][i] = 1;
	}
	for (int j = 0; j < n; j++) {

        #pragma omp parallel for
		for (int i = j; i < n; i++) {
			double sum = 0;
			for (int k = 0; k < j; k++) {
				sum = sum + L[i][k] * U[k][j];	
			}
			L[i][j] = A[i][j] - sum;
		}

        #pragma omp parallel for
		for (int i = j; i < n; i++) {
			double sum = 0;
			for(int k = 0; k < j; k++) {
				sum = sum + L[j][k] * U[k][i];
			}
			if (L[j][j] == 0) {		
				printf("Exiting program !!!\n");		
				exit(0);
			}
			U[j][i] = (A[j][i] - sum) / L[j][j];
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