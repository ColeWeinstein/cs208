/*
    puzzle3.c
    Cole Weinstein
    A function which copies one string into another (not accounting for string overflow).
*/

void function1(char *n, char *m) {
    if (n == 0 || m == 0) {
        return;
    }
    while(*m != 0) {
        *n = *m;
        n++;
        m++;
    }
    *n = *m;
}