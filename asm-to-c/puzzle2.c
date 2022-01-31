/*
    puzzle2.c
    Cole Weinstein
    A program which compares three integers and returns the largest one.
*/

int function1 (int n, int m) {
    if (n <= m) {
        n = m;
    }
    return n;
}

int function2 (int x, int y, int z) {
    int n = function1(x, y);
    return function1(z, n);
}