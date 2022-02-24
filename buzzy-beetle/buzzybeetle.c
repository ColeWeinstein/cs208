/*
    buzzybeetle.c
    Written by Cole Weinstein, 2/21/2022.
    Adapted from Jeff Ondich's 'shell208.c', 'exectest.c', 'forktest.c',
    'pipe.c', and 'redirect.c' sample code, written 16 February 2022.

    Currently a version for the buzzybtl shell.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_COMMAND_LINE_LENGTH 100

int bb_argc = 0;
int bb_argc2 = 0;
char *bb_argv[MAX_COMMAND_LINE_LENGTH];
char *bb_argv2[MAX_COMMAND_LINE_LENGTH];

int pipe_bool = 0;
int redirect_input_bool = 0;
int redirect_output_bool = 0;

char *file_name;

char *help_message = "Thank you for using the buzzybeetle shell.\n"
                     "\n"
                     "This shell accepts the following commands:\n"
                        "\t- simple Unix commands: enter a command and any desired flags and parameters\n"
                        "\t- singly piped Unix commands: enter 'command_1 | command_2'"
                        " to connect the output of command 1 to the input of command 2\n"
                        "\t- singly redirected input: enter 'command < file_name' to redirect"
                        " the input of the command to read from file_name\n"
                        "\t- singly redirected output: enter 'command > file_name' to redirect"
                        " the output of the command to file file_name\n"
                            "\t\t- note, if file_name exists, it will be completely rewritten,"
                            " otherwise the shell will create a new file named file_name\n"
                        "\t- 'h', 'help': print this help document\n"
                        "\t- 'q', 'quit': exit the shell\n";

void initialize();
int help();
void arg_parser(char *command_line);
void free_args();
void execute_command(char *command_line);
void print_args();
void cleanup();

int main() {
    initialize();

    // The main infinite loop
    char command_line[MAX_COMMAND_LINE_LENGTH];
    while (1) {
        printf("buzzybtl$ ");
        if (fgets(command_line, MAX_COMMAND_LINE_LENGTH, stdin) != NULL) {
            execute_command(command_line);
        } else {
            printf("Something went wrong. Try again, I guess.\n");
        }
    }

    return 0;
}

void execute_command(char *command_line) {
    if (strcmp(command_line, "q\n") == 0 || strcmp(command_line, "quit\n") == 0){
        free_args();
        exit(0);
    }

    if (strcmp(command_line, "h\n") == 0 || strcmp(command_line, "help\n") == 0) {
        printf("%s", help_message);
        free_args();
        return;
    }

    arg_parser(command_line);


    pid_t pid = fork();

    if (pid != 0) {
        /* Parent */
	    int status;
        pid = wait(&status);

    } else {
		/* Child */
        if (pipe_bool) {
            int fd[2];
            if (pipe(fd) < 0) {
                perror("Trouble creating pipe");
                fflush(stderr);
                free_args();
                return;
            }

            pid_t split_pid = fork();

            if(split_pid != 0) {
                /* Child Command 1 */
                close(fd[0]);
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    perror("Trouble redirecting stdout");
                    fflush(stderr);
                    free_args();
                    return;
                }
                close(fd[1]);

                execvp(bb_argv[0], bb_argv);
                perror("exec of first command failed");
                fflush(stderr);
            } else {
                /* Child Command 2 */
                close(fd[1]);
                if (dup2(fd[0], STDIN_FILENO) == -1) {
                    perror("Trouble redirecting stdin");
                    fflush(stderr);
                    free_args();
                    return;
                }
                close(fd[0]);

                execvp(bb_argv2[0], bb_argv2);
                perror("exec of first command failed");
                fflush(stderr);
            }
        } else {
            if (redirect_input_bool) {
                int fd = open(file_name, O_RDONLY, 0644);
                if (fd < 0) {
                    perror("Trouble opening file");
                    fflush(stderr);
                    free_args();
                    return;
                }

                if (dup2(fd, STDIN_FILENO) < 0) {
                    perror("Trouble dup2-ing to stdin");
                    fflush(stderr);
                    close(fd);
                    free_args();
                    return;
                }

                close(fd);

                execvp(bb_argv[0], bb_argv);
                perror("exec failed");
                fflush(stderr);
            } else if (redirect_output_bool) {
                int fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("Trouble opening file");
                    fflush(stderr);
                    free_args();
                    return;
                }

                if (dup2(fd, STDOUT_FILENO) < 0) {
                    perror("Trouble dup2-ing to stdout");
                    fflush(stderr);
                    close(fd);
                    free_args();
                    return;
                }

                close(fd);

                execvp(bb_argv[0], bb_argv);
                perror("exec failed");
                fflush(stderr);
            } else {
                execvp(bb_argv[0], bb_argv);
                perror("exec failed");
                fflush(stderr);
            }
            
            
        }
    }

    free_args();
}

void arg_parser(char *command_line) {
    char *command_copy = malloc(strlen(command_line));
    strcpy(command_copy, command_line);
    char *input_file = strtok(command_copy, "<");
    if(strcmp(input_file, command_line) != 0) {
        redirect_input_bool = 1;
        strcpy(command_line, input_file);
        input_file = strtok(NULL, ">");
        file_name = malloc(strlen(input_file));
        strcpy(file_name, input_file);
    }

    strcpy(command_copy, command_line);
    char *output_file = strtok(command_copy, ">");
    if (strcmp(output_file, command_line) != 0) {
        redirect_output_bool = 1;
        strcpy(command_line, output_file);
        output_file = strtok(NULL, ">");
        file_name = malloc(strlen(output_file));
        strcpy(file_name, output_file);
    }

    free(command_copy);

    char *command_1 = strtok(command_line, "|");
    char *command_2 = strtok(NULL, "|");
    if (command_2 != NULL) {
        pipe_bool = 1;
    }

    char *token = " \n";

    bb_argc = 0;
    char *curr_arg = strtok(command_1, token);
    while (curr_arg != NULL) {
        bb_argv[bb_argc] = malloc(strlen(curr_arg) * sizeof(char));
        strcpy(bb_argv[bb_argc], curr_arg);
        curr_arg = strtok(NULL, token);
        bb_argc++;
    }

    bb_argv[bb_argc] = NULL;


    // if pipe in command
    bb_argc2 = 0;
    curr_arg = strtok(command_2, token);
    while (curr_arg != NULL) {
        bb_argv2[bb_argc2] = malloc(strlen(curr_arg) * sizeof(char));
        strcpy(bb_argv2[bb_argc2], curr_arg);
        curr_arg = strtok(NULL, token);
        bb_argc2++;
    }

    bb_argv2[bb_argc2] = NULL;

    if (redirect_input_bool || redirect_output_bool) {
        if (file_name[0] == 0x20) {
            strcpy(file_name, file_name+1);
        }
        strncpy(file_name+(strlen(file_name)-1), "\0", 1);
    }
}

void print_args() {
    printf("bb_argc = %d\n", bb_argc);
    for (int i = 0; i < bb_argc; i++) {
        printf("bb_argv[%d] = %s\n", i, bb_argv[i]);
    }
}

void free_args() {
    for (int i = 0; i < bb_argc; i++) {
        free(bb_argv[i]);
    }

    if (pipe_bool) {
        for (int i = 0; i < bb_argc2; i++) {
            free(bb_argv2[i]);
        }
        pipe_bool = 0;
    }

    if (redirect_input_bool || redirect_output_bool) {
        free(file_name);
    }

    redirect_input_bool = 0;
    redirect_output_bool = 0;
}

int help() {
    FILE *help_file = fopen("help.txt", "r");
    if (help_file == NULL) {
        return 1;
    }

    char ch = fgetc(help_file);
    while (ch != EOF) {
        fprintf(stdout, "%c", ch);
        ch = fgetc(help_file);
    }

    fprintf(stdout, "\n");

    if (ferror(help_file)) {
        fclose(help_file);
        return 1;
    }

    fclose(help_file);

    return 0;
}

void initialize() {
    // set bb_argv array to be empty
    for (int i = 0; i < MAX_COMMAND_LINE_LENGTH; i++) {
        bb_argv[i] = NULL;
    }
}