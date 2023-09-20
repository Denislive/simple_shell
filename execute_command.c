#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * execute_command - Execute a command with arguments
 * @command: The command to execute
 *
 * Return: 1 on success, -1 on failure
 */
int execute_command(char *command)
{
    char *token = strtok(command, " ");
    char *args[1024];
    int i = 0;

    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (execvp(args[0], args) == -1)
    {
        perror("Error executing command");
        return -1;
    }

    return 1;
}
