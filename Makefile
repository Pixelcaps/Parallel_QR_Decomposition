serial:
	gcc -Wall -g serial_QR.c -o serial -lm
omp:
	gcc -Wall -g -fopenmp openMP_QR.c -o parallel -lm
omp_tasks:
	gcc -Wall -g -fopenmp openMP_QR_tasks.c -o parallel_tasks -lm
clean:
	rm parallel serial parallel_tasks

