#include "utils.h"
#include "strats.h"

int main() {
    // TODO : Optimize input
    
    int N = 2000; 
    char fname[] = "tests/input_2000.txt";
    int num_threads = 4;

    double **A,**L,**U;
    A = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) A[i] = malloc(N*sizeof(double));
    L = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) L[i] = malloc(N*sizeof(double));
    U = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) U[i] = malloc(N*sizeof(double));

    // clear_matrix(A, N);
    // clear_matrix(L, N);
    // clear_matrix(U, N);

    FILE *f = fopen(fname, "r");
    char str[20];
    for(int i = 0; i < N*N; ++i){
        fscanf(f,"%s", str);
        A[i/N][i%N] = atof(str);
    }
    fclose(f);

	// print_matrix(A, N); printf("\n");
    
    // crout_0(A,L,U,N, num_threads);
    // crout_1(A,L,U,N, num_threads);
    crout_2(A,L,U,N, num_threads);
    // crout_3(A,L,U,N, num_threads);

	// print_matrix(L, N); printf("\n");
	// print_matrix(U, N); printf("\n");

    write_output("outputs/output_L.txt", L, N);
    write_output("outputs/output_U.txt", U, N);

    return 0;
}