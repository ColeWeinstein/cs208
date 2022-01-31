/*
    puzzle4.c
    Cole Weinstein
    A simple function to calculate the nth Fibonacci number.
*/

int function1(int n) {
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return n;
    }
    int x = (n-1) + function1(n-2);
    return x;
}