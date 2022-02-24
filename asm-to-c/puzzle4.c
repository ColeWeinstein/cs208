/*
    puzzle4.c
    Cole Weinstein
    A simple function to calculate the nth Fibonacci number via recursion. The function starts with n and makes recursive calls to n-1 and n-2 before summing up how many relevant function calls have been made in total.
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