#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

#define epsilon 0.0000000001
#define tau 0.00001
#define N 10

int i, j;
double A[N][N];
double x[N];
double b[N];
double Ax[N];
double error, normAxDivB, normB;
int chunkSize;

void init() {
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = (i == j) ? 2 : 1;
        }
        b[i] = N + 1;
        x[i] = 0;
        Ax[i] = 0;
        normB += b[i] * b[i];
    }
    normB = sqrt(normB);
}

void nonParallel() {
    do {
        for (i = 0; i < N; i++) {
            x[i] -= tau * (Ax[i] - b[i]);
        }

        for (i = 0; i < N; i++) {
            printf("%lf ", x[i]);
        }

        for (i = 0; i < N; i++) {
            Ax[i] = 0;
            for (j = 0; j < N; j++) {
                Ax[i] += A[i][j] * x[j];
            }
        }

        normAxDivB = 0;
        for (i = 0; i < N; i++) {
            normAxDivB += (Ax[i] - b[i]) * (Ax[i] - b[i]);
        }
        printf("%lf", normAxDivB);
        exit(0);
        error = sqrt(normAxDivB) / normB;
    } while (error >= epsilon);
}

void oneParallelBlock(int numberOfThreads) {
    omp_set_num_threads(numberOfThreads);
    #pragma omp parallel
    do {
        #pragma omp for schedule(static, chunkSize)
        for (i = 0; i < N; i++) {
            x[i] -= tau * (Ax[i] - b[i]);
        }

        #pragma omp for schedule(static, chunkSize) private(j)
        for (i = 0; i < N; i++) {
            Ax[i] = 0;
            for (j = 0; j < N; j++) {
                Ax[i] += A[i][j] * x[j];
            }
        }

        normAxDivB = 0;
        #pragma omp for schedule(static, chunkSize) reduction(+:normAxDivB)
        for (i = 0; i < N; i++) {
            normAxDivB += (Ax[i] - b[i]) * (Ax[i] - b[i]);
        }

        #pragma omp single
        {
            error = sqrt(normAxDivB) / normB;
        }
    } while (error > epsilon);
}

void multipleParallelBlocks(int numberOfThreads) {
    omp_set_num_threads(numberOfThreads);
    do {
        #pragma omp parallel for schedule(static, chunkSize)
        for (i = 0; i < N; i++) {
            x[i] -= tau * (Ax[i] - b[i]);
        }

        #pragma omp parallel for schedule(static, chunkSize) private(j)
        for (i = 0; i < N; i++) {
            Ax[i] = 0;
            for (j = 0; j < N; j++) {
                Ax[i] += A[i][j] * x[j];
            }
        }

        normAxDivB = 0;
        #pragma omp parallel for schedule(static, chunkSize) reduction(+:normAxDivB)
        for (i = 0; i < N; i++) {
            normAxDivB += (Ax[i] - b[i]) * (Ax[i] - b[i]);
        }
        error = sqrt(normAxDivB) / normB;
    } while (error > epsilon);
}

int checkAnswer() {
    for (i = 0; i < N; i++) {
        if ((int) round(x[i]) != 1) {
            return 0;
        }
    }
    return 1;
}

int main() {

    for (int threadsNum = 1; threadsNum <= 12; threadsNum++) {
        chunkSize = N / threadsNum;
        init();
        struct timespec start_time, end_time;
        double elapsed_time;

        clock_gettime(CLOCK_MONOTONIC, &start_time);

//        multipleParallelBlocks(threadsNum);
        nonParallel();
//        oneParallelBlock(threadsNum);

        clock_gettime(CLOCK_MONOTONIC, &end_time);
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                       (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

        printf("\nElapsed time: %lf\n", elapsed_time);
        printf("Answer is correct: %d", checkAnswer());
    }
}
