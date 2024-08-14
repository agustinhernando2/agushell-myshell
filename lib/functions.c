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

    int r = snprintf(buffer, sizeof(buffer), YELLOW "%s@" GRAY "%s:~%s$" BLUE ">", username, hostname, pwd);
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

int exec_command(char* command)
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
        if (clear_shell() == -1)
        {
            perror(RED "Error: clear_shell" NORMAL);
            return -1;
        }
        return 0;
    }
    else if ((strcmp(cmd_token, "quit") == 0) || (strcmp(cmd_token, "exit") == 0))
    {
        exit_shell();
        return 0;
    }
    else if ((strcmp(cmd_token, "cd") == 0))
    {
        char* path = strtok(NULL, "");
        if (path == NULL)
        {
            perror(RED "Error: path is NULL" NORMAL);
            return 0;
        }
        // clear \n if exists
        if (path[strlen(path) - 1] == '\n')
        {
            path[strlen(path) - 1] = '\0';
        }
        char* pathto = get_directory(path);
        if (pathto == NULL)
        {
            perror(RED "Error: get_directory" NORMAL);
            return 0;
        }
        exchange_directory(pathto);
        return 0;
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
            return -1;
        }
        else if (r == -2)
        {
            perror(RED "Error: snprintf" NORMAL);
            return -1;
        }
        else if (r == -3)
        {
            perror(RED "Error: msg is NULL" NORMAL);
            return -1;
        }
        else if (r == -4)
        {
            perror(RED "Error: bufflen is 0" NORMAL);
            return -1;
        }
        else
        {
            printf("%s\n", buffer);
            return 0;
        }
    }
    else
    {
        extern_command(get_arguments_for_extern_command(command));
        return 0;
    }
}

int clear_shell()
{
    int r = system("clear");
    if (r == -1)
    {
        return -1;
    }
    return 0;
}

void exit_shell()
{
    running = 0;
}

char* get_directory(char* path)
{
    static char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    if (path == NULL)
    {
        return NULL;
    }

    if (strcmp(path, "-") == 0)
    {
        return path;
    }

    // check if the token is a relative path
    if (strstr(getenv(PWD), path) != NULL)
    {
        strcat(buffer, "/");
        strcat(buffer, path);
    }
    // check if the token is an absolute path
    else
    {
        strcat(buffer, getenv(PWD));
        strcat(buffer, "/");
        strcat(buffer, path);
    }
    return buffer;
}

int set_directory(char* command)
{
    if (chdir(command) != 0)
    {
        perror(RED "Error" NORMAL);
        return -1;
    }
    return 0;
}

int exchange_directory(char* path)
{
    char* old_pwd = getenv(OLDPWD);
    char* pwd = getenv(PWD);

    // check if dir is "-" to go to the previous directory
    if (strcmp(path, "-") == 0)
    {
        if (old_pwd == NULL)
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
        if (setenv(PWD, old_pwd, 1))
        {
            perror(RED "Error" NORMAL);
            return -1;
        }

        if (pwd == NULL)
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
        if (setenv(OLDPWD, pwd, 1))
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
        if (chdir(old_pwd) != 0)
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
        return 0;
    }

    // the command is a directory, check if it exists
    if (chdir(path) != 0)
    {
        perror(RED "Error" NORMAL);
        return -1;
    }
    else
    {
        // set the old directory to the current directory
        if (setenv(OLDPWD, pwd, 1))
        {
            perror(RED "Error" NORMAL);
            return -1;
        }

        char* new_pwd = getcwd(pwd, PATH_MAX);
        if (new_pwd == NULL)
        {
            perror(RED "Error" NORMAL);
            return -1;
        }

        // set the current directory to the new directory
        if (setenv(PWD, new_pwd, 1))
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
    }
    return 0;
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
            int r = snprintf(buffer, bufflen, RED "Environment not found: %s" NORMAL, msg + 1);
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

            int r =
                snprintf(buffer, bufflen, "comment: msglen[%ld], commlen[%ld], %s", msglen, strlen(comment), comment);
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
        perror(RED "fork" NORMAL);
        exit(EXIT_FAILURE);
        break;
    case 0:
        argumentos = c;
        char* path = malloc(100 * sizeof(char));
        strcat(path, "/bin/");
        strcat(path, argumentos[0]);
        if (execv(path, argumentos) == -1)
        {
            perror(RED "Error" NORMAL);
        }
        exit(EXIT_SUCCESS);
        break;
    default:
        wait(0);
        break;
    }
}
