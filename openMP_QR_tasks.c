#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int k, i, j;
	int m, n;
	double **r, **q, **a, mysum;

	m = 5;
	n = 5;

	a = malloc(sizeof(double *) * m);
	q = malloc(sizeof(double *) * m);
	r = malloc(sizeof(double *) * n);

	for (i = 0; i < m; i++) {
		a[i] = malloc(sizeof(double) * n);
		q[i] = malloc(sizeof(double) * n);
		for (j = 0; j < n; j++)
			a[i][j] = i*j + 500;
	}
	for (i = 0; i < n; i++) {
		r[i] = malloc(sizeof(double) * n);
	}
	for (k = 0; k < n; k++){
		r[k][k] = 0;
		mysum = 0;
		
		#pragma omp parallel
		{
			#pragma omp single
			{
				for (i = 0; i < m; i++) {
					#pragma omp task private(i) reduction(+:mysum)
					mysum += a[i][k] * a[i][k];
				}
			}
		
			#pragma omp taskwait

			r[k][k] = mysum;
			r[k][k] = sqrt(r[k][k]);

			#pragma omp single
			{

				for (i = 0; i < m; i++) {			
					#pragma omp task private(i) shared(k,r)
					q[i][k] = a[i][k] / r[k][k];
				}
			}

			#pragma omp taskwait

			#pragma omp single
			{
				for(j = k + 1; j < n; j++) {
					mysum = 0;
					#pragma omp task private(j, mysum) shared(k, r)
					{
						for(i = 0; i < m; i++) {
							#pragma	omp task private(i) shared(k, j)
							mysum += q[i][k] * a[i][j];
						}

						#pragma omp taskwait	

						r[k][j] = mysum;

						for (i = 0; i < m; i++) {
							#pragma omp task private(i) shared(k, j)
							a[i][j] = a[i][j] - r[k][j] * q[i][k];
						}					
						#pragma omp taskwait
					}
				}
			}
			#pragma omp taskwait
		}
	}


	printf("a = \n");

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%lf ", a[i][j]);
		}
		printf("\n");
	}

	printf("\n\n");

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%lf ", q[i][j]);
		}
		printf("\n");
	}

	printf("\n\n");

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%lf ", r[i][j]);
		}
		printf("\n");
	}
		
	printf("\n\n");

	for (i = 0; i < m; i++) {
		free(a[i]);
		free(q[i]);
	}
	for (i = 0; i < n; i++) {
		free(r[i]);
	}
	free(a);
	free(q);
	free(r);
	return 0;
}
