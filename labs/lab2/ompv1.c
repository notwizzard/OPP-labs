#include <stdio.h>
#include <math.h>
#include <omp.h>

#define epsilon 0.00001
#define tau 0.001
#define N 100

int main() {

    int i, j;
    double A[N][N], x[N], b[N], Ax[N], error, normAxDivB, normB;

    struct timespec start_time, end_time;
    double elapsed_time;

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    // Init start values
    #pragma omp parallel for private(j)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = (i == j) ? 2 : 1;
        }
        b[i] = N + 1;
        x[i] = 0;
        Ax[i] = 0;
    }



    clock_gettime(CLOCK_MONOTONIC, &end_time);
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                   (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%lf ", x[i]);
        }
        printf("\n");
    }

    printf("\n%lf", elapsed_time);

}
