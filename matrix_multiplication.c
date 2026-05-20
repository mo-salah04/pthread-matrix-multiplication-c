#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 3

int A[N][N] = {
    {0, 1, 2},
    {1, 2, 3},
    {2, 3, 4}
};

int B[N][N] = {
    {0, -1, -2},
    {1, 0, -1},
    {2, 1, 0}
};

int C_std[N][N];
int C_elem[N][N];
int C_row[N][N];

typedef struct {
    int row;
    int col;
} ElementArgs;

typedef struct {
    int row;
} RowArgs;

static void standard_mult(void) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C_std[i][j] = sum;
        }
    }
}

static void *compute_element(void *arg) {
    ElementArgs *data = (ElementArgs *)arg;
    int sum = 0;

    for (int k = 0; k < N; ++k) {
        sum += A[data->row][k] * B[k][data->col];
    }

    C_elem[data->row][data->col] = sum;
    free(data);
    return NULL;
}

static void *compute_row(void *arg) {
    RowArgs *data = (RowArgs *)arg;
    int row = data->row;

    for (int j = 0; j < N; ++j) {
        int sum = 0;
        for (int k = 0; k < N; ++k) {
            sum += A[row][k] * B[k][j];
        }
        C_row[row][j] = sum;
    }

    free(data);
    return NULL;
}

static void print_matrix(const char *label, int matrix[N][N]) {
    printf("%s\n", label);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

static int matrices_equal(int x[N][N], int y[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (x[i][j] != y[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

static void create_element_threads(void) {
    pthread_t threads[N * N];
    int t = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            ElementArgs *args = malloc(sizeof(ElementArgs));
            if (args == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            args->row = i;
            args->col = j;

            if (pthread_create(&threads[t], NULL, compute_element, args) != 0) {
                perror("pthread_create");
                free(args);
                exit(EXIT_FAILURE);
            }
            ++t;
        }
    }

    for (int i = 0; i < N * N; ++i) {
        pthread_join(threads[i], NULL);
    }
}

static void create_row_threads(void) {
    pthread_t threads[N];

    for (int i = 0; i < N; ++i) {
        RowArgs *args = malloc(sizeof(RowArgs));
        if (args == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        args->row = i;

        if (pthread_create(&threads[i], NULL, compute_row, args) != 0) {
            perror("pthread_create");
            free(args);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }
}

int main(void) {
    printf("Matrix Multiplication Using POSIX Threads\n");
    printf("Matrix size: %dx%d\n", N, N);

    print_matrix("\nMatrix A:", A);
    print_matrix("\nMatrix B:", B);

    standard_mult();
    print_matrix("\nStandard multiplication result:", C_std);

    create_element_threads();
    print_matrix("\nVersion 1 - Element-per-thread result:", C_elem);

    create_row_threads();
    print_matrix("\nVersion 2 - Row-per-thread result:", C_row);

    if (matrices_equal(C_std, C_elem) && matrices_equal(C_std, C_row)) {
        printf("\nCheck: all methods produced the same result.\n");
    } else {
        printf("\nCheck: mismatch found between multiplication methods.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
