#include<stdlib.h>
#include<stdio.h>

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

void clear_matrix(double **A, int N) {
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            A[i][j] = 0;
        }
    }
}