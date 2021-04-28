#include "utils.h"
#include "strats.h"

int main(int argc, char *argv[]) {
    // TODO : Optimize input
    
    int N = atoi(argv[1]);
    char fname[100];
    strcpy(fname, argv[2]);
    int num_threads = atoi(argv[3]);
    int strategy = atoi(argv[4]);

    double **A,**L,**U;
    A = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) A[i] = malloc(N*sizeof(double));
    L = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) L[i] = malloc(N*sizeof(double));
    U = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) U[i] = malloc(N*sizeof(double));

    clear_matrix(A, N);
    clear_matrix(L, N);
    clear_matrix(U, N);

    FILE *f = fopen(fname, "r");
    char str[20];
    for(int i = 0; i < N*N; ++i){
        fscanf(f,"%s", str);
        A[i/N][i%N] = atof(str);
    }
    fclose(f);

	// print_matrix(A, N); printf("\n");
    
    switch(strategy){
        case 0: crout_0(A,L,U,N, num_threads);   //48s
                break;
        case 1: crout_1(A,L,U,N, num_threads);   //26s  
                break;
        case 2: crout_2(A,L,U,N, num_threads);   //38s
                break;
        case 3: crout_3(A,L,U,N, num_threads);   //42s
                break;
        default: printf("ERROR\n");
    }

	// print_matrix(L, N); printf("\n");
	// print_matrix(U, N); printf("\n");

    char outL[50]; sprintf(outL, "output_L_%d_%d.txt", strategy, num_threads);
    char outU[50]; sprintf(outU, "output_U_%d_%d.txt", strategy, num_threads);

    write_output(outL, L, N);
    write_output(outU, U, N);

    return 0;
}