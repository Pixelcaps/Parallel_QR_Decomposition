#include <iostream>
#include <vector>
#include <math.h>
#include <omp.h>
#include "tbb/tbb.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"

using namespace std;
using namespace tbb;

typedef std::vector< std::vector<double> > matrix;

int main(int argc, char *argv[]) {
	// int k, i, j;
	int m, n;
	double mysum;
	tbb::task_scheduler_init init(4);
	
	n = 2500;
	m = 2500;

	matrix r(n, std::vector<double>(n));
	matrix a(m, std::vector<double>(n));
	matrix q(m, std::vector<double>(n));

	
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			a[i][j] = i*j + 500;
	}

	parallel_for(blocked_range <size_t>(0, n), 
		[&](const blocked_range<size_t>& b) {
			for (size_t k = b.begin(); k!=b.end(); k++){
				r[k][k] = 0;
				mysum = 0;
				for (int i = 0; i < m; i++)
					mysum += a[i][k] * a[i][k];
				r[k][k] = mysum;
				r[k][k] = sqrt(r[k][k]);
				for (int i = 0; i < m; i++)
					q[i][k] = a[i][k] / r[k][k];
				for(int j = k + 1; j < n; j++) {
					mysum = 0;
					for(int i = 0; i < m; i++)
						mysum += q[i][k] * a[i][j];
					r[k][j] = mysum;
					for (int i = 0; i < m; i++)
						a[i][j] = a[i][j] - r[k][j] * q[i][k];
				}
			}
		});
	return 0;
}



