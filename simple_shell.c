#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(void)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    pid_t child_pid;
    int status;

    char *exec_args[] = {NULL, NULL};
    char *exec_env[] = {NULL};

    while (1)
    {
        printf("$ ");
        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        if (bytes_read == -1)
        {
            perror("Error reading command");
            exit(EXIT_FAILURE);
        }

        if (bytes_read == 0)
        {
            putchar('\n');
            break;
        }

        buffer[bytes_read - 1] = '\0';

        child_pid = fork();

        if (child_pid == -1)
        {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0)
        {
            exec_args[0] = buffer;

            if (execve(buffer, exec_args, exec_env) == -1)
            {
                perror("Error executing command");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(&status);
        }
    }

    return (0);
}
