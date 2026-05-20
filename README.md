# Matrix Multiplication Using POSIX Threads

A C program that compares normal matrix multiplication with two multithreaded versions using POSIX threads.

## Implemented Methods

1. Standard matrix multiplication without threads.
2. Element-per-thread multiplication, where each output element is calculated by one thread.
3. Row-per-thread multiplication, where each row of the output matrix is calculated by one thread.

## Files

```text
pthread-matrix-multiplication-c-github/
├── matrix_multiplication.c
├── Makefile
├── README.md
├── .gitignore
└── docs/
    └── OS-Lab3-2.pdf
```

## Build

```bash
make
```

## Run

```bash
./matrix_multiplication
```

## Expected Result

All three methods should produce the same matrix result:

```text
5 2 -1
8 2 -4
11 2 -7
```

The program prints a final check message confirming whether all methods match.
