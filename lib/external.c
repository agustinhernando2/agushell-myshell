#include "../include/external.h"

int jobid = 1;
pid_t child_pid = 0;

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

void extern_command(char* input)
{

    if (strchr(input, '<') != NULL && strchr(input, '>') != NULL)
    {
        char* token = strtok(input, "<");
        int counter = 0;
        char* program = NULL;
        char* redirectin = NULL;
        char* redirectout = NULL;
        while (token != NULL)
        {
            if (counter == 0)
            {
                program = (char*)malloc(sizeof(token));
                strcpy(program, token);
            }
            else if (counter == 1)
            {
                int counter2 = 0;
                char* token2 = strtok(token, ">");
                while (token2 != NULL)
                {
                    remove_spaces(token2);
                    if (counter2 == 0)
                    {
                        redirectin = (char*)malloc(sizeof(token2));
                        strcpy(redirectin, token2);
                    }
                    else if (counter2 == 1)
                    {
                        redirectout = (char*)malloc(sizeof(token2));
                        strcpy(redirectout, token2);
                    }
                    token2 = strtok(NULL, ">");
                    counter2++;
                }
                free(token2);
            }
            token = strtok(NULL, "<");
            counter++;
        }
        free(token);
        if (program == NULL)
        {
            fprintf(stderr, "Error, comando invalido\n");
        }
        else
        {
            if (strchr(program, '|') != NULL)
            {
                execute_pipes(program, redirectin, redirectout);
            }
            else
            {
                execute_binary(program, redirectin, redirectout);
            }
        }
        free(program);
        free(redirectin);
        free(redirectout);
    }
    else if (strchr(input, '<') != NULL)
    {
        int c = 0;
        char* token = strtok(input, "<");
        char* program = NULL;
        char* redirectin = NULL;
        while (token != NULL)
        {
            if (c == 0)
            {
                program = (char*)malloc(sizeof(token));
                strcpy(program, token);
            }
            else if (c == 1)
            {
                remove_spaces(token);
                redirectin = (char*)malloc(sizeof(token));
                strcpy(redirectin, token);
            }
            else
            {
                fprintf(stderr, "Error, invalid command\n");
            }

            token = strtok(NULL, "<");
            c++;
        }
        free(token);
        if (program == NULL)
        {
            fprintf(stderr, "Error, comando invalido\n");
        }
        else
        {
            if (strchr(program, '|') != NULL)
            {
                execute_pipes(program, redirectin, NULL);
            }
            else
            {
                execute_binary(program, redirectin, NULL);
            }
        }
        free(program);
        free(redirectin);
    }
    else if (strchr(input, '>') != NULL)
    {
        int c = 0;
        char* token = strtok(input, ">");
        char* program = NULL;
        char* redirectout = NULL;
        while (token != NULL)
        {
            if (c == 0)
            {
                program = (char*)malloc(sizeof(token));
                strcpy(program, token);
            }
            else if (c == 1)
            {
                remove_spaces(token);
                redirectout = (char*)malloc(sizeof(token));
                strcpy(redirectout, token);
            }
            else
            {
                fprintf(stderr, "Error, invalid command\n");
            }

            token = strtok(NULL, ">");
            c++;
        }

        free(token);
        if (program == NULL)
        {
            fprintf(stderr, "Error, comando invalido\n");
        }
        else
        {
            if (strchr(program, '|') != NULL)
            {
                execute_pipes(program, NULL, redirectout);
            }
            else
            {
                execute_binary(program, NULL, redirectout);
            }
        }

        free(program);
        free(redirectout);
    }
    else
    {
        if (strchr(input, '|') != NULL)
        {
            execute_pipes(input, NULL, NULL);
        }
        else
        {
            execute_binary(input, NULL, NULL);
        }
    }clear
}

void execute_pipes(char input[], char* redirectin, char* redirectout)
{
    return;
}

void execute_binary(char* input, char* redirectin, char* redirectout)
{
    signal(SIGCHLD, handler);
    signal(SIGTSTP, handler);
    signal(SIGQUIT, handler);
    signal(SIGINT, handler);

    // check if the command should be executed in the background
    // if the last character is &, the command should be executed in the background
    int8_t is_background = 0;
    if (input[strlen(input) - 1] == '&')
    {
        is_background = 1;
        input[strlen(input) - 1] = '\0';
    }

    int pid = fork();
    if (pid < 0)
    {
        perror(RED "Error" NORMAL);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // child process
        if (redirectin != NULL)
        {
            // redirect stdin
            if (freopen(redirectin, "r", stdin) == NULL)
            {
                perror(RED "Error redirecting stdin" NORMAL);
            }
        }
        if (redirectout != NULL)
        {
            // redirect stdout
            if (freopen(redirectout, "w", stdout) == NULL)
            {
                perror(RED "Error redirecting stdout" NORMAL);
            }
        }

        char* args[MAX_ARGS];
        parse_input(input, args);
        /**
         * Las funciones execlp y execvp duplicarán las acciones del shell al buscar un
         * fichero ejecutable si el nombre de fichero especificado no contiene un carácter
         * de barra inclinada (/). El camino de búsqueda es el especificado en el entorno
         * por la variable PATH. Si esta variable no es especificada, se emplea el camino
         * predeterminado ``:/bin:/usr/bin''. Además, ciertos errores se tratan de forma especial.
         */

        // execl no busca en PATH
        // if (execl(args[0], args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9],
        // args[10], NULL) < 0) {
        //     perror("execl failed");
        //     exit(EXIT_FAILURE);
        // }

        if (execvp(args[0], args) < 0)
        {
            // Error al ejecutar el comando
            perror(RED "execvp failed" NORMAL);
            exit(EXIT_FAILURE);
        }
        // if execvp is successful, this code will not be reached
        perror(RED "An error occurred in execution,the path is incorrect or some argument is invalid" NORMAL);
        exit(EXIT_FAILURE);
    }
    else
    {
        child_pid = pid;
    }

    if (!is_background)
    {
        // wait for the child process to finish
        // waitpid(pid, NULL, 0);
        pause();
    }
    printf("[%d]%d\n", jobid, child_pid);
}

void parse_input(char* input, char** args)
{
    for (int i = 0; i < MAX_ARGS; i++)
    {
        args[i] = strsep(&input, " ");
        if (args[i] == NULL)
            break;
    }
}

void handler(int signum)
{
    // ctrl-c, ctrl-z, ctrl-|
    switch (signum)
    {
    case SIGCHLD:
        // signal received when a child process changes state
        waitpid(-1, 0, WNOHANG);
        jobid++;
        break;
    case SIGINT:
        // signal received when the user presses ctrl-c
        if (child_pid > 0)
        {
            signal(SIGINT, SIG_IGN);
            printf("\n");
            kill(child_pid, SIGINT);
        }
        break;
    case SIGTSTP:
        // signal received when the user presses ctrl-z
        if (child_pid > 0)
        {
            signal(SIGTSTP, SIG_IGN);
            printf("\nSTOP[%d]\n", child_pid);
            kill(child_pid, SIGTSTP);
        }
        break;
    case SIGQUIT:
        // signal received when the user presses ctrl-|
        if (child_pid > 0)
        {
            signal(SIGQUIT, SIG_IGN);
            printf("\n");
            kill(child_pid, SIGQUIT);
        }
        break;
    }
}
