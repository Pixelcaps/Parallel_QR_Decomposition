using namespace std;

typedef std::vector< std::vector<double> > matrix;

int main(int argc, char *argv[]) {
	int k, i, j;
	int m, n;
	double mysum;

	n = 2500;
	m = 2500;

	matrix r(n, std::vector<double>(n));
	matrix a(m, std::vector<double>(n));
	matrix q(m, std::vector<double>(n));

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			a[i][j] = i*j + 500;
	}

	for (k = 0; k < n; k++){
		r[k][k] = 0;
		mysum = 0;
		cilk::reducer< cilk::op_add<double> > parallel_sum(0.0);
		cilk_for (int i = 0; i < m; i++)
			*parallel_sum += a[i][k] * a[i][k];
		mysum = parallel_sum.get_value();
		r[k][k] = mysum;
		r[k][k] = sqrt(r[k][k]);
		cilk_for (int i = 0; i < m; i++)
			q[i][k] = a[i][k] / r[k][k];
		cilk_for(int j = k + 1; j < n; j++) {
			mysum = 0;
			cilk::reducer< cilk::op_add<double> > parallel_sumator(0.0);
			cilk_for(int i = 0; i < m; i++)
				*parallel_sumator += q[i][k] * a[i][j];
			r[k][j] = parallel_sumator.get_value();
			cilk_for (int i = 0; i < m; i++)
				a[i][j] = a[i][j] - r[k][j] * q[i][k];
		}
	}

	for (i = 0; i < n; i++) {
		cout << endl;
		for (j = 0; j < n; j++) {
			cout << r[i][j] << " ";
		}
	}
	cout << endl;
	for (i = 0; i < m; i++) {
		cout << endl;
		for (j = 0; j < n; j++) {
			cout << q[i][j] << " "; 
		}
	}
	cout << endl;

	return 0;
}