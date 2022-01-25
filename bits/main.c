/*
    main.c
    Jeff Ondich, 19 January 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bits.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 7) {
        fprintf(stderr, "Usage: %s action [arguments...]\n", argv[0]);
        fprintf(stderr, "  where action can be to_upper, to_lower, middle_bits...\n");
        fprintf(stderr, "  and arguments are appropriate to the specific action.\n");
        return 1;
    }

    if (strcmp(argv[1], "to_upper") == 0) {
        if (argc != 3) {
            fprintf(stderr, "to_upper requires 1 argument: string_to_capitalize\n");
            return 1;
        }
        char *buffer = malloc(strlen(argv[2]) + 1);
        if (buffer != NULL) {
            strcpy(buffer, argv[2]);
            to_upper(buffer);
            printf("%s\n", buffer);
            free(buffer);
        }
    }

    else if (strcmp(argv[1], "to_lower") == 0) {
        if (argc != 3) {
            fprintf(stderr, "to_lower requires 1 argument: string_to_lowercase\n");
            return 1;
        }
        char *buffer = malloc(strlen(argv[2]) + 1);
        if (buffer != NULL) {
            strcpy(buffer, argv[2]);
            to_lower(buffer);
            printf("%s\n", buffer);
            free(buffer);
        }
    }

    else if (strcmp(argv[1], "middle_bits") == 0) {
        if (argc != 5) {
            fprintf(stderr, "middle_bits requires 3 arguments: n, bit_index, bit_count\n");
            return 1;
        }
        char *buffer_1 = argv[2];
        int n = atoi(buffer_1);
        char *buffer_2 = argv[3];
        int bit_index = atoi(buffer_2);
        char *buffer_3 = argv[4];
        int bit_count = atoi(buffer_3);
        int result = middle_bits(n, bit_index, bit_count);
        if (n != 0 && result == n) {
            fprintf(stderr, "Error. Either n, bit_index, or bit_count was negative, bit_index was larger than 30, or bit_count exceeded the size of the integer.\n");
            return 1;
        }
        printf("\nresult = %d\n", result);
    }

    else if (strcmp(argv[1], "to_utf8") == 0) {
        if (argc != 3) {
            fprintf(stderr, "to_utf8 requires 1 argument: codepoint\n");
            return 1;
        }
        char *utf8_buffer = malloc(0);
        // if (utf8_buffer == NULL) {
        //     fprintf(stderr, "Error allocating memory to utf8_buffer.\n");
        //     return 1;
        // }
        char *codepoint_argv = argv[2];
        printf("%s\n", codepoint_argv);
        int codepoint = atoi(codepoint_argv);
        printf("0x%X\n", codepoint);
        bool success = to_utf8(codepoint, utf8_buffer);
        if (!success) {
            printf("Error in codepoint.\n");
            return 1;
        }
        int i = 0;
        while (utf8_buffer[i] != 0) {
            printf("%X ", (unsigned char) utf8_buffer[i]);
            i++;
        }
        printf("\n%s\n", utf8_buffer);
        free(utf8_buffer);
    }

    else if (strcmp(argv[1], "from_utf8") == 0) {
        if (argc < 2 || argc > 7) {
            fprintf(stderr, "from_utf8 requires 0-5 arguments: up to 4 utf8_buffer chars\n");
            return 1;
        }
        char *utf8_buffer = malloc(5);
        if (utf8_buffer == NULL) {
            fprintf(stderr, "Unable to allocate memory to utf8_buffer.\n");
            return 1;
        }
        utf8_buffer[0] = (char) 0xF7;
        utf8_buffer[1] = (char) 0x87;
        utf8_buffer[2] = (char) 0x87;
        utf8_buffer[3] = (char) 0x87;
        utf8_buffer[4] = (char) 0x00;

        int i;
        for (i = 0; i < 5; i++) {
            printf("%X ", (unsigned char) utf8_buffer[i]);
        }
        printf("\n");

        int codepoint = from_utf8(utf8_buffer);

        printf("U+%X\n", codepoint);

    }

    // etc.

    return 0;
}
