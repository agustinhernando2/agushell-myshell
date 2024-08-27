#include "../include/common.h"

int exec_command(char* command)
{
    static char buffer[BUFFER_SIZE];

    // clear \n if exists
    if (command[strlen(command) - 1] == '\n')
    {
        command[strlen(command) - 1] = '\0';
    }
    // save the command
    char* cmd = (char*)malloc(strlen(command) + 1);
    if (cmd == NULL)
    {
        perror(RED "Error: malloc" NORMAL);
        return -1;
    }
    strcpy(cmd, command);

    char* cmd_token = strtok(command, SPACE);

    if ((strcmp(cmd_token, "clr") == 0) || (strcmp(cmd_token, "clear") == 0))
    {
        if (clear_shell() == -1)
        {
            perror(RED "Error: clear_shell" NORMAL);
            goto bad_end;
        }
        goto ok_end;
    }
    else if ((strcmp(cmd_token, "quit") == 0) || (strcmp(cmd_token, "exit") == 0))
    {
        exit_shell();
        goto ok_end;
    }
    else if ((strcmp(cmd_token, "cd") == 0))
    {
        char* path = strtok(NULL, "");
        if (path == NULL)
        {
            perror(RED "Error: path is NULL" NORMAL);
            goto ok_end;
        }
        // clear \n if exists
        if (path[strlen(path) - 1] == '\n')
        {
            path[strlen(path) - 1] = '\0';
        }
        exchange_directory(path);
        goto ok_end;
    }
    else if ((strcmp(cmd_token, "echo") == 0))
    {
        char* msg = strtok(NULL, "");

        // clear \n if exists
        if (msg[strlen(msg) - 1] == '\n')
        {
            msg[strlen(msg) - 1] = '\0';
        }

        int r = echo_shell(msg, strlen(msg), buffer, sizeof(buffer));
        if (r == -1)
        {
            fprintf(stderr, "%s\n", buffer);
            goto bad_end;
        }
        else if (r == -2)
        {
            perror(RED "Error: snprintf" NORMAL);
            goto bad_end;
        }
        else if (r == -3)
        {
            perror(RED "Error: msg is NULL" NORMAL);
            goto bad_end;
        }
        else if (r == -4)
        {
            perror(RED "Error: bufflen is 0" NORMAL);
            goto bad_end;
        }
        else
        {
            printf("%s\n", buffer);
            goto ok_end;
        }
    }
    else
    {
        // extern_command(get_arguments_for_extern_command(command));
        extern_command(cmd);
        goto ok_end;
    }
bad_end: // free memory
    free(cmd);
    return -1;
ok_end: // free memory
    free(cmd);
    return 0;
}

void remove_char(char* str, char charToRemmove)
{
    int i, j;
    int len = strlen(str);
    for (i = 0; i < len; i++)
    {
        if (str[i] == charToRemmove)
        {
            for (j = i; j < len; j++)
            {
                str[j] = str[j + 1];
            }
            len--;
            i--;
        }
    }
}

void remove_spaces(char* str1)
{
    int count = 0;
    for (int i = 0; str1[i]; i++)
        if (str1[i] != ' ')
            str1[count++] = str1[i];
    str1[count] = '\0';
}

void remove_beginning_spaces(char* str1)
{
    char* str2;
    str2 = str1;
    while (*str2 == ' ')
        str2++;
    if (str2 != str1)
        memmove(str1, str2, strlen(str2) + 1);
}
