#include "../include/functions.h"

// Definir las variables

int running = 1;
// char *token;
char* const* argumentos;
char pwd[PATH_MAX];

char* generate_prompt(char* username, char* hostname)
{
    static char buffer[BUFFER_SIZE];

    if (getcwd(pwd, sizeof(pwd)) == NULL)
    {
        return NULL;
    }

    int r = snprintf(buffer, sizeof(buffer), AMARILLO "%s@" GRIS "%s:~%s$" CELESTE ">", username, hostname, pwd);
    if (r < 0 || r >= sizeof(buffer))
    {
        return NULL;
    }

    // setenv(PWD, pwd, 1);

    return buffer;
}

int get_running()
{
    return running;
}

void exec_command(char* command)
{
    static char buffer[BUFFER_SIZE];
    char* cmd_token = strtok(command, SPACE);
    
    // clear \n if exists
    if (cmd_token[strlen(cmd_token) - 1] == '\n')
    {
        cmd_token[strlen(cmd_token) - 1] = '\0';
    }   

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

        // clear \n if exists
        if (msg[strlen(msg) - 1] == '\n')
        {
            msg[strlen(msg) - 1] = '\0';
        }

        int r = echo_shell(msg, strlen(msg), buffer, sizeof(buffer));
        if (r == -1)
        {
            fprintf(stderr, "%s\n", buffer);
        }
        else if (r == -2)
        {
            perror(ROJO "Error: snprintf");
        }
        else if (r == -3)
        {
            perror(ROJO "Error: msg is NULL");
        }
        else if (r == -4)
        {
            perror(ROJO "Error: bufflen is 0");
        }
        else
        {
            printf("%s\n", buffer);
        }
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

int echo_shell(char* msg, size_t msglen, char* buffer, size_t bufflen)
{

    if (msg == NULL)
    {
        return -3;
    }
    if (bufflen == 0)
    {
        return -4;
    }
    // compare if the first character is a dollar sign
    if (strncmp(msg, DOLLAR, 1) == 0)
    {
        // get the environment variable
        char* env = getenv(msg + 1);
        if (env != NULL)
        {
            snprintf(buffer, bufflen, "%s", env);
            return 0;
        }
        else
        {
            int r = snprintf(buffer, bufflen, ROJO "Environment not found: %s", msg + 1);
            if (r < 0 || r >= bufflen)
            {
                return -2;
            }
            return -1;
        }
    }
    else
    {
        // check if the message is a comment
        if (msg[0] == '"' && (msg[msglen - 1] == '"'))
        {
            char comment[msglen - 1];
            for (int i = 0; i < msglen - 1; i++)
            {
                comment[i] = msg[i + 1];
            }
            comment[msglen - 2] = '\0';

            int r = snprintf(buffer, bufflen, "comment: msglen[%ld], commlen[%ld], %s", msglen, strlen(comment),
                             comment);
            if (r < 0 || r >= bufflen)
            {
                return -2;
            }
            return 0;
        }
        else
        {
            snprintf(buffer, bufflen, "%s", msg);
            return 0;
        }
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
