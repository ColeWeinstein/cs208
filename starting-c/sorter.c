/* 
    sorter.c
    Written by Cole Weinstein for Carleton's CS208
    1/10/2022

    This program reads in a text file, sorts its lines lexographically, and prints the sorted file to stdout.
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments supplied. Please run this program using the following command:\n\t./sorter textfile\n");
        return 1;
    }
    else if (argc > 2) {
        fprintf(stderr, "Too many arguments supplied. Please run this program using the following command:\n\t./sorter textfile\n");
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error. Unable to open input file.\n");
        return 1;
    }

    // 2D array to store text file. Assumed no more than 500 lines, and no more than 200 bytes per line (ignoring new line characters and accounting for null terminating chars, we maintain a maximum array length of 200).
    char file_contents[500][200];
    char ch;
    int i, j;
    for (i = 0; i < 500; i++) {
        for(j = 0; j < 200; j++) {
            ch = fgetc(input_file);
            // break out of this inner loop if we've reached the end of the line or the EOF char. We don't want to add either of them to the array (we don't want to print EOF to output and manually add new line characters later), so we check for it here. Also, we add the null character to terminate the string (relevant for empty lines).
            if (ch == EOF || ch == 10) {
                file_contents[i][j] = 0;
                break;
            }

            file_contents[i][j] = ch;
        }
        
        // break out of the loop if we've reached the EOF.
        if (ch == EOF) {
            break;
        }
    }

    if (ferror(input_file)) {
        fprintf(stderr, "Error reading contents of file.\n");
        return 1;
    }

    int x, y;
    int index_min;
    char temp_string[200];
    // since the reading of the file (or writing of the array) stopped at row i, we only have to compare the first i rows of the array, not all 500.
    for (x = 0; x <= i; x++) {
        index_min = x;
        for (y = x; y <= i; y++) {
            // if we are on an empty line (ie. file_contents[y] is an empty string), move to the next line.
            if (file_contents[y][0] == 0) {
                continue;
            }
            // if we are currently keeping track of an empty line, instead keep track of this line.
            else if (file_contents[index_min][0] == 0) {
                index_min = y;
            }
            // normal string comparison, swapping if necessary. previous checks ensure neither of these lines are empty strings.
            else if (strncmp(file_contents[y], file_contents[index_min], 200) < 0) {
                index_min = y;
            }
        }
        strcpy(temp_string, file_contents[x]);
        strcpy(file_contents[x], file_contents[index_min]);
        strcpy(file_contents[index_min], temp_string);
    }

    // we can reset j, since it's final state wasn't important. however, the var i stores the line in the file where EOF was. thus, if we only iterate through the array until line i, we can avoid constantly checking for EOF.
    for (j = 0; j <= i; j++) {
        fprintf(stdout, "%s", file_contents[j]);
        if (j != i) {
            fprintf(stdout, "\n");
        }
    }

    fclose(input_file);

    return 0;
}