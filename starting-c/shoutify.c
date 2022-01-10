/*
    shoutify.c
    Written by Cole Weinstein for Carleton's CS208
    1/10/2022

    This program takes an input file and writes its contents in uppercase to an output file.
*/

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Not enough arguments supplied. Please run this program using the following command:\n\t./shoutify inputfile outputfile\n");
    }
    else if (argc > 3) {
        fprintf(stderr, "Too many arguments supplied. Please run this program using the following command:\n\t./shoutify inputfile outputfile\n");
    }
    
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error. Unable to open input file.\n");
        return 1;
    }
    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error. Unable to open output file.\n");
        return 1;
    }
    
    char ch = fgetc(input_file);
    while (ch != EOF) {
        if (ch > 96 && ch < 122) {
            ch -= 32;
        }
        fputc(ch, output_file);
        ch = fgetc(input_file);
    }

    if (ferror(input_file)) {
        fclose(input_file);
        fclose(output_file);
        fprintf(stderr, "Error. Unable to read file.\n");
        return 1;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}