#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * main - Entry point for the simple shell program.
 *
 * Description: This program implements a simple shell that takes user
 *              commands and executes them using execve.
 *
 * Return: Always returns EXIT_SUCCESS.
 */
int main(void) {
    char *line = NULL;
    size_t len = 0;
    char *args[2];

    while (1) {
        printf("#cisfun$ ");
        if (getline(&line, &len, stdin) == -1) {
            
            printf("\n");
            break;
        }

        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        args[0] = line;
        args[1] = NULL;

        if (execve(line, args, NULL) == -1) {
            perror("Error");
        }
    }

    free(line);
    return EXIT_SUCCESS;
}

