#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define MAX_INPUT_SIZE 1024

void display_prompt() {
    printf("#cisfun$ ");
}

int main() {
    char input[MAX_INPUT_SIZE];
    char **env = environ;
    char *path = getenv("PATH");

    while (1) {
        display_prompt();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* Handle Ctrl+D (end of file) */
            printf("\n");
            break;
        }

        /* Remove the trailing newline character */
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            /* Handle the "exit" command */
            break;
        } else if (strcmp(input, "env") == 0) {
            /* Handle the "env" command */
            while (*env != NULL) {
                printf("%s\n", *env);
                env++;
            }
        } else {
            pid_t pid = fork();

            if (pid == -1) {
                perror("fork");
            } else if (pid == 0) {
                /* Child process */
                char *args[MAX_INPUT_SIZE / 2];
                int i = 0;
                char *token = strtok(input, " ");
                char *path_copy;
                char *dir;

                while (token != NULL) {
                    args[i++] = token;
                    token = strtok(NULL, " ");
                }

                args[i] = NULL;

                /* Check if the command exists in the PATH */
                path_copy = strdup(path);
                dir = strtok(path_copy, ":");

                while (dir != NULL) {
                    char command_path[MAX_INPUT_SIZE];
                    snprintf(command_path, sizeof(command_path), "%s/%s", dir, args[0]);

                    if (access(command_path, X_OK) == 0) {
                        if (execve(command_path, args, NULL) == -1) {
                            perror(args[0]);
                            exit(EXIT_FAILURE);
                        }
                    }

                    dir = strtok(NULL, ":");
                }

                /* If the loop finishes and the command is not found, exit */
                free(path_copy);
                fprintf(stderr, "%s: command not found\n", args[0]);
                exit(EXIT_FAILURE);
            } else {
                /* Parent process */
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    return 0;
}
