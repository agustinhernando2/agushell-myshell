#include "../include/internal.h"

int running = 1;
// char *token;
char pwd[PATH_MAX];

char* generate_prompt(char* username, char* hostname)
{
    static char buffer[BUFFER_SIZE];

    if (getcwd(pwd, sizeof(pwd)) == NULL)
    {
        return NULL;
    }
    // if pwd starts with /home/username, replace it with ~
    // strstr() is to search for a substring within a larger string
    // and it helps to find the first occurrence of a specified substring.
    if (strstr(pwd, getenv("HOME")) == pwd)
    {
        // extract the substring
        memmove(pwd, pwd + strlen(getenv("HOME")), 1 + strlen(pwd + strlen(getenv("HOME"))));
        char* tilde = "~";
        char* temp = malloc(strlen(tilde) + strlen(pwd) + 1);
        strcpy(temp, tilde);
        strcat(temp, pwd);
        // update the pwd
        strcpy(pwd, temp);
    }

    int r = snprintf(buffer, sizeof(buffer), YELLOW "%s@" GRAY "%s:%s$" BLUE ">", username, hostname, pwd);
    if (r < 0 || r >= sizeof(buffer))
    {
        return NULL;
    }

    return buffer;
}

int get_running()
{
    return running;
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

int exchange_directory(char* path)
{
    char* old_pwd = NULL;
    char* pwd = NULL;
    char* temp = NULL;

    if (strcmp(path, "") == 0)
    {
        // change to home directory
        if ((chdir(getenv("HOME"))) != 0)
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
    }
    // check if the path is a directory
    else if (strcmp(path, "-") != 0)
    {
        old_pwd = getcwd(NULL, 0);
        if (old_pwd == NULL)
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
        // change to the new directory
        if (chdir(path) != 0)
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
        // set the old directory to the current directory
        if (setenv(OLDPWD, old_pwd, 1) != 0)
        {
            free(old_pwd);
            perror(RED "Error" NORMAL);
            return -1;
        }

        free(old_pwd);
    }
    // check the instruction is "-"
    else if (strcmp(path, "-") == 0)
    {
        temp = getenv(OLDPWD);
        if (temp == NULL)
        {
            perror(RED "Error" NORMAL);
            return -1;
        }
        old_pwd = getenv(PWD);
        if (old_pwd == NULL)
        {
            perror(RED "Error" NORMAL);
            free(temp);
            return -1;
        }
        // change to the old directory
        if (chdir(temp) != 0)
        {
            perror(RED "Error" NORMAL);
            free(temp);
            return -1;
        }
        // set the old directory to the current directory
        if (setenv(OLDPWD, old_pwd, 1) != 0)
        {
            perror(RED "Error" NORMAL);
            free(temp);
            return -1;
        }
        return 0;
    }

    pwd = getcwd(NULL, 0);
    if (pwd == NULL)
    {
        perror(RED "Error" NORMAL);
        return -1;
    }

    // update the current directory environment variable
    if (setenv(PWD, pwd, 1) != 0)
    {
        perror(RED "Error" NORMAL);
        free(pwd);
        return -1;
    }
    free(pwd);
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
