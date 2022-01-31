/*
    puzzle5.c
    Cole Weinstein
    A simple function to compute the nth Fibonacci number.
*/

int function1(int n) {
    if (n <= 0) {
        return 0;
    }
    int fib_curr = 0;
    int fib_next = 1;
    int fib_prev;
    for (int i = 1; i < n; i++) {
        fib_prev = fib_next;
        fib_next = fib_next + fib_curr;
        fib_curr = fib_prev;
    }
    return fib_next;
}