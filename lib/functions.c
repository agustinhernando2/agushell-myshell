#include "../include/functions.h"

// Definir las variables

int running = 1;
// char *token;
char* const* argumentos;
char pwd[PATH_MAX];

void show_prompt(char* username, char* hostname)
{
    if (getcwd(pwd, sizeof(pwd)) == NULL)
    {
        perror("Error");
    }
    setenv(PWD, pwd, 1);
    // ansi colors https://talyian.github.io/ansicolors/
    printf(AMARILLO "%s@" GRIS "%s:~%s$" CELESTE ">", username, hostname, pwd);
}

int get_running()
{
    return running;
}

void exec_command(char* command)
{
    char* cmd_token = strtok(command, SPACE);

    if ((strcmp(cmd_token, "clr") == 0) || (strcmp(cmd_token, "clear") == 0))
    {
        clear_shell();
    }
    else if ((strcmp(cmd_token, "quit") == 0) || (strcmp(cmd_token, "exit") == 0))
    {
        exit_shell();
    }
    else if ((strcmp(cmd_token, "cd ") == 0))
    {
        exchange_directory(get_directory(command));
    }
    else if ((strcmp(cmd_token, "echo") == 0))
    {
        char* msg = strtok(NULL, "");
        echo_shell(msg, strlen(msg));
    }
    else
    {
        extern_command(get_arguments_for_extern_command(command));
    }
}

void clear_shell()
{
    system("clear");
    // printf(CLEAR);
}

void exit_shell()
{
    running = 0;
}

char* get_directory(char* command)
{
    char* token = strtok(command, SPACE);
    if (token != NULL)
    {
        token = strtok(NULL, "\n");
    }
    char* arg = malloc(500 * sizeof(char));
    if (strcmp(token, "-") == 0)
    {
        return token;
    }
    if (strstr(getenv(PWD), token) != NULL)
    {
        strcat(arg, "/");
        strcat(arg, token);
    }
    else
    {
        strcat(arg, getenv(PWD));
        strcat(arg, "/");
        strcat(arg, token);
    }
    return arg;
}

void exchange_directory(char* dir)
{
    if (strcmp(dir, "-") == 0)
    {
        dir = getenv(OLDPWD);
    }
    setenv(OLDPWD, pwd, 1);
    if (chdir(dir) != 0)
    {
        perror(ROJO "Error");
    }
    else
    {
        setenv(PWD, getcwd(pwd, 256), 1);
    }
}

void echo_shell(char* msg, size_t len)
{
    if (msg == NULL)
    {
        return;
    }
    // compare if the first character is a dollar sign
    if (strncmp(msg, DOLLAR, 1) == 0)
    {
        // get the environment variable
        char* env = getenv(msg + 1);
        if (env != NULL)
        {
            printf("%s\n", env);
        }
        else
        {
            fprintf(stderr, ROJO "Error: Environment variable not found");
            printf("\n");
        }
    }
    else
    {
        // check if the message is a comment
        if (msg[0] == '"' && (msg[len - 2] == '"'))
        {
            char comment[len - 2];
            for (int i = 0; i < len - 3; i++)
            {
                comment[i] = msg[i + 1];
            }
            comment[len - 3] = '\0';
            printf("comment: %ld, %ld   %s\n", len, strlen(comment), comment);
        }
        else
            printf("%s\n", msg);
    }
}

void get_command(char* command)
{
    fgets(command, CMD_MAX, stdin);
}

char** get_arguments_for_extern_command(char* command)
{
    char** arg = malloc(50 * sizeof(char*));
    int index = 0;

    char* token = strtok(command, "\n");
    arg[index] = strdup(token);
    token = strtok(arg[index], " ");
    if (token != NULL)
    {
        arg[index] = strdup(token);
    }
    else
    {
        arg[index + 1] = NULL;
        return arg;
    }
    index++;
    while (token && (token = strtok(NULL, " ")))
    {
        arg[index] = strdup(token);
        index++;
    }
    arg[index] = NULL;
    return arg;
}

void extern_command(char* c[])
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror(ROJO "fork");
        exit(EXIT_FAILURE);
        break;
    case 0:
        argumentos = c;
        char* path = malloc(100 * sizeof(char));
        strcat(path, "/bin/");
        strcat(path, argumentos[0]);
        if (execv(path, argumentos) == -1)
        {
            perror(ROJO "Error");
        }
        exit(EXIT_SUCCESS);
        break;
    default:
        wait(0);
        break;
    }
}
