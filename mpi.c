#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"


int main(int argc, char *argv[]) {
    
    int pid, np, elements_per_process, num_recvd;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    // printf("%d\n",pid);
    
    // TODO : Optimize input
    
    int N = atoi(argv[1]); 
    char fname[100];
    strcpy(fname, argv[2]);
    
    //working A,L,U
    double *A,*L,*U;
    A = malloc(N*N*sizeof(double));
    L = malloc(N*N*sizeof(double));
    U = malloc(N*N*sizeof(double));

    double **_L,**_U;    

    if(pid == 0){
        // written LU
        _L = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) _L[i] = malloc(N*sizeof(double));
        _U = malloc(N*sizeof(double *)); for(int i=0; i < N; i++) _U[i] = malloc(N*sizeof(double));

        FILE *f = fopen(fname, "r");
        char str[20];
        for(int i = 0; i < N*N; ++i){
            fscanf(f,"%s", str);
            A[i] = atof(str);
        }
        fclose(f);
    }
    MPI_Bcast(A, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    /* MPI BEGIN */
    

    double *L1, *U1;
    L1 = malloc(N*N*sizeof(double));
    U1 =  malloc(N*N*sizeof(double));

    for (int i = 0; i < N; i++) 
		U[i*N+i] = 1;
    
    for(int j = 0; j < N; ++j){
        MPI_Bcast(U, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        elements_per_process = (N-j)/np;
        if(pid == 0){
            int elements_left;
            if(np > 1){
                for (int i = 1; i < np - 1; i++) {
                    int index = (i*elements_per_process + j)*N;
                    
                    MPI_Send(&elements_per_process,
                                1, MPI_INT, i, 0,
                                MPI_COMM_WORLD);
                    MPI_Send(&L[index],
                                elements_per_process*N,
                                MPI_DOUBLE, i, 0,
                                MPI_COMM_WORLD);
                }
                int index = ((np-1)*elements_per_process + j)*N;
                elements_left = (N*N - index)/N;
                MPI_Send(&elements_left,
                        1, MPI_INT,
                        np-1, 0,
                        MPI_COMM_WORLD);
                MPI_Send(&L[index],
                        elements_left*N,
                        MPI_DOUBLE, np-1, 0,
                        MPI_COMM_WORLD);
            }
            double sum = 0;
            for (int i = j; i < elements_per_process+j; i++) {
                sum = 0;
                for (int k = 0; k < j; k++) {
                    sum = sum + L[i*N + k] * U[j*N + k];	
                }
                L[i*N + j] = A[i*N + j] - sum;
		    }

            //recieve
            double *tmp = malloc(N*N*sizeof(double));
            if(np > 1){
                for(int i = 1; i < np-1; ++i){
                    MPI_Recv(&L[(i*elements_per_process + j)*N], elements_per_process*N, MPI_DOUBLE,
                        i, 0,
                        MPI_COMM_WORLD,
                        &status);
                }
                MPI_Recv(&L[((np-1)*elements_per_process + j)*N], elements_left*N, MPI_DOUBLE,
                    np-1, 0,
                    MPI_COMM_WORLD,
                    &status);
            }
            if (L[j*N + j] == 0) {		
				printf("Exiting program !!!\n");		
				exit(0);
			}
        }
        else{
            MPI_Recv(&num_recvd,
                 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD,
                 &status);
            MPI_Recv(&L1[j*N], num_recvd*N,
                 MPI_DOUBLE, 0, 0,
                 MPI_COMM_WORLD,
                 &status);
            double sum = 0;
            for (int i = j; i < num_recvd+j; i++) {
                sum = 0;
                for (int k = 0; k < j; k++) {
                    sum = sum + L1[i*N + k] * U[j*N + k];	
                }
                L1[i*N + j] = A[(i+pid*elements_per_process)*N + j] - sum;
		    }
            MPI_Send(&L1[j*N],
                num_recvd*N,
                MPI_DOUBLE, 0, 0,
                MPI_COMM_WORLD);
            
            // clear_matrix2(L1, N*N);
        }

        //loop2
        MPI_Bcast(L, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if(pid == 0){
            int elements_left;
            if(np > 1){
                for (int i = 1; i < np - 1; i++) {
                    int index = (i*elements_per_process + j)*N;
                    
                    MPI_Send(&elements_per_process,
                                1, MPI_INT, i, 0,
                                MPI_COMM_WORLD);
                    MPI_Send(&U[index],
                                elements_per_process*N,
                                MPI_DOUBLE, i, 0,
                                MPI_COMM_WORLD);
                }
                int index = ((np-1)*elements_per_process + j)*N;
                elements_left = (N*N - index)/N;
    
                MPI_Send(&elements_left,
                        1, MPI_INT,
                        np-1, 0,
                        MPI_COMM_WORLD);
                MPI_Send(&U[index],
                        elements_left*N,
                        MPI_DOUBLE, np-1, 0,
                        MPI_COMM_WORLD);
            }
            double sum = 0;
            for (int i = j; i < elements_per_process+j; i++) {
                sum = 0;
                for (int k = 0; k < j; k++) {
                    sum = sum + L[j*N + k] * U[i*N + k];	
                }
                U[i*N + j] = (A[j*N + i] - sum) / L[j*N + j];
		    }

            //recieve
            double *tmp = malloc(N*N*sizeof(double));
            if(np > 1){
                for(int i = 1; i < np-1; ++i){
                    MPI_Recv(&U[(i*elements_per_process + j)*N], elements_per_process*N, MPI_DOUBLE,
                        i, 0,
                        MPI_COMM_WORLD,
                        &status);
                }
                MPI_Recv(&U[((np-1)*elements_per_process + j)*N], elements_left*N, MPI_DOUBLE,
                    np-1, 0,
                    MPI_COMM_WORLD,
                    &status);
            }
        }
        else{
            MPI_Recv(&num_recvd,
                 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD,
                 &status);
            MPI_Recv(&U1[j*N], num_recvd*N,
                 MPI_DOUBLE, 0, 0,
                 MPI_COMM_WORLD,
                 &status);
            double sum = 0;
            for (int i = j; i < num_recvd+j; i++) {
                sum = 0;
                for (int k = 0; k < j; k++) {
                    sum = sum + L[j*N + k] * U1[i*N + k];	
                }
                U1[i*N + j] = (A[j*N + (i+pid*elements_per_process)] - sum) / L[j*N + j];
		    }
            MPI_Send(&U1[j*N],
                num_recvd*N,
                MPI_DOUBLE, 0, 0,
                MPI_COMM_WORLD);
            
            // clear_matrix2(U1, N*N);
        }

    }


    /* MPI END */
    if(pid == 0){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                _L[i][j] = L[i*N+j];
                _U[i][j] = U[j*N+i];
            }
        }
        char outL[50] = "output_L_4_";
        char outU[50] = "output_U_4_";
        char threadstring[50]; sprintf(threadstring, "%d.txt", np);
        strcat(outL,threadstring);
        strcat(outU,threadstring);
        write_output(outL, _L, N);
        write_output(outU, _U, N);
    }

    MPI_Finalize();
    return 0;
}