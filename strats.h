#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

void crout_0(double **A, double **L, double **U, int n, int no_threads) {
	int i, j, k;
	double sum = 0;
	for (i = 0; i < n; i++) {
		U[i][i] = 1;
	}
	for (j = 0; j < n; j++) {
		for (i = j; i < n; i++) {
			sum = 0;
			for (k = 0; k < j; k++) {
				sum = sum + L[i][k] * U[k][j];	
			}
			L[i][j] = A[i][j] - sum;
		}
		for (i = j; i < n; i++) {
			sum = 0;
			for(k = 0; k < j; k++) {
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

void crout_1(double **A, double **L, double **U, int n, int no_threads) {
    
	for (int i = 0; i < n; i++) {
		U[i][i] = 1;
	}
	for (int j = 0; j < n; j++) {

        #pragma omp parallel for num_threads(no_threads)
		for (int i = j; i < n; i++) {
			double sum = 0;
			for (int k = 0; k < j; k++) {
				sum = sum + L[i][k] * U[k][j];	
			}
			L[i][j] = A[i][j] - sum;
		}

        #pragma omp parallel for num_threads(no_threads)
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

/* 3rd Strategy to to do crout */
void crout_2(double **A, double **L, double **U, int n, int no_threads) {

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

        omp_set_nested(1);
        omp_set_num_threads(2); // fixed

        #pragma omp parallel sections
        {   
            #pragma omp section
            {   
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

/* 3rd Strategy to to do crout */
void crout_3(double **A, double **L, double **U, int n, int no_threads) {

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

        omp_set_nested(1);
        omp_set_num_threads(2); // fixed

        #pragma omp parallel sections
        {   
            #pragma omp section
            {   
                #pragma omp parallel num_threads(no_threads/2)
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
                #pragma omp parallel num_threads(no_threads - no_threads/2)
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