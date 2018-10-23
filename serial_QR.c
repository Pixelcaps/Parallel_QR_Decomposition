#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int k, i, j;
	int m, n;
	double **r, **q, **a, mysum;

	n = 3000;
	m = 3000;

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
		for (i = 0; i < m; i++)
			mysum += a[i][k] * a[i][k];
		r[k][k] = mysum;
		r[k][k] = sqrt(r[k][k]);
		for (i = 0; i < m; i++)
			q[i][k] = a[i][k] / r[k][k];
		for(j = k + 1; j < n; j++) {
			mysum = 0;
			for(i = 0; i < m; i++)
				mysum += q[i][k] * a[i][j];
			r[k][j] = mysum;
			for (i = 0; i < m; i++)
				a[i][j] = a[i][j] - r[k][j] * q[i][k];
		}
	}
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