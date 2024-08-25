#include "../include/external.h"
#include "external.h"

int jobid = 1;
pid_t child_pid = 0;

// extern command, aca se interpreta si hay pipes en el input
void extern_command(char* input)
{
    int8_t is_background = 0;
    // get amount of pipes that the input has
    int pipes = 0;

    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '|')
        {
            pipes++;
        }
    }
    // alloc memory for the pipes
    int* pipe_array = (int*)malloc((pipes + 1) * 2 * sizeof(int));
    if (pipe_array == NULL)
    {
        perror(RED "Error" NORMAL);
        exit(EXIT_FAILURE);
    }

    // create pipes
    for (int i = 0; i < (pipes + 1); i++)
    {
        if (pipe(pipe_array + i * 2) == -1)
        {
            perror(RED "Error" NORMAL);
            exit(EXIT_FAILURE);
        }
    }

    // make a copy of the input
    char* program = strdup(input);

    char* token = strtok(program, "|");

    int flag = 0;
    for (size_t i = 0; i < (pipes + 1); i++)
    {
        int pid = fork();
        if (pid < 0)
        {
            perror(RED "Error" NORMAL);
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // child process

            // check if the command should be executed in the background
            // if the last character is &, the command should be executed in the background
            if (input[strlen(input) - 1] == '&')
            {
                is_background = 1;
                input[strlen(input) - 1] = '\0';
            }

            // remove the last character if it is a newline or space
            if (input[strlen(input) - 1] == '\n' || input[strlen(input) - 1] == ' ')
            {
                input[strlen(input) - 1] = '\0';
            }

            process_command(token, is_background);

            // if execvp is successful, this code will not be reached

            perror(RED "An error occurred in execution,the path is incorrect or some argument is invalid" NORMAL);
            exit(EXIT_FAILURE);
        }
        else
        {
            // parent process
            child_pid = pid;
        }

        if (!is_background)
        {
            // wait for the child process to finish
            // waitpid(pid, NULL, 0);
            pause();
        }
        printf("[%d]%d\n", jobid, child_pid);

        token = strtok(NULL, "|");
        if (token == NULL)
        {
            break;
        }
    }
}

// process_command, aca se interpreta si hay redirecciones en el input
void process_command(char* input, int8_t is_background)
{
    char *token = NULL, *redirectin = NULL, *redirectout = NULL;

    // remove the last character if it is a newline or space
    if (input[strlen(input) - 1] == '\n' || input[strlen(input) - 1] == ' ')
    {
        input[strlen(input) - 1] = '\0';
    }

    char* program = strdup(input);

    // buscar la primera ocurrencia de < o >, obtener el caracter
    // si el caracter es <, obtener el nombre del archivo y guardar en redirectin
    // si el caracter es >, obtener el nombre del archivo y guardar en redirectout
    // si la primer ocurrencia fue <, buscar la siguiente ocurrencia de >
    // y obtener el nombre del archivo y guardar en redirectout

    if (strchr(input, '<') != NULL || strchr(input, '>') != NULL)
    {
        // find the first occurrence of < or >
        // ex: ls > a.out -> ls /0 a.out
        // ex: cat < a.out -> cat /0 a.out
        token = parse_token(program, "<>");
        if (token == NULL)
        {
            fprintf(stderr, "Error, comando invalido\n");
            return;
        }

        if (strchr(input, '<') != NULL)
        {
            // get file name after < and > if it exists
            token = strtok(NULL, "<");
            handle_redirections(token, &redirectin, &redirectout);
        }
        else
        {
            // get file name after >
            token = strtok(NULL, ">");
            remove_spaces(token);
            redirectout = strdup(token);
        }

        set_redirect(redirectin, "r", stdin);
        set_redirect(redirectout, "w", stdout);
        // execute the command
        execute_binary(program, is_background);
    }
    else
    {
        // execute the command without redirection
        execute_binary(input, is_background);
    }
}
// execute_binary, aca se ejecuta el comando binario
void execute_binary(char* program, int8_t is_background)
{
    // remove the last character if it is a newline or space
    if (program[strlen(program) - 1] == '\n' || program[strlen(program) - 1] == ' ')
    {
        program[strlen(program) - 1] = '\0';
    }

    if (is_background)
    {
        // ignore signals in the child process
        signal(SIGCHLD, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, SIG_IGN);

        // redirect stdout and stderr to /dev/null
        freopen("/dev/null", "w", stdout);
        freopen("/dev/null", "w", stderr);
    }

    char* args[MAX_ARGS];
    parse_input(program, args);
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
}

void set_redirect(char* redirectin, char* c, FILE* stream)
{
    if (redirectin != NULL)
    {
        // redirect stdin
        if (freopen(redirectin, c, stream) == NULL)
        {
            perror(RED "Error redirecting stdin" NORMAL);
        }
    }
}

char* parse_token(char* input, const char* delimiter)
{
    char* token = strtok(input, delimiter);
    if (token != NULL)
    {
        // return a copy of the token
        return strdup(token);
    }
    return NULL;
}

void handle_redirections(char* token, char** redirectin, char** redirectout)
{
    int counter = 0;
    char* token2;

    token2 = strtok(token, ">");
    while (token2 != NULL)
    {
        remove_spaces(token2);
        if (counter == 0)
        {
            *redirectin = strdup(token2);
        }
        else if (counter == 1)
        {
            *redirectout = strdup(token2);
        }
        token2 = strtok(NULL, ">");
        counter++;
    }
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
        // Signal received when a child process changes state (e.g., exits)
        // -1: Wait for any child process
        // WNOHANG: Return immediately if no child process has exited
        // This wait is needed to avoid zombie processes by collecting
        // the exit status of the child process
        waitpid(-1, 0, WNOHANG);
        jobid++;
        break;
    case SIGINT:
        // signal received when the user presses ctrl-c
        if (child_pid > 0)
        {
            signal(SIGINT, SIG_IGN);
            printf("\nSIGINT sended to [%d]\n", child_pid);
            sleep(1);
            kill(child_pid, SIGINT);
        }
        break;
    case SIGTSTP:
        // signal received when the user presses ctrl-z
        if (child_pid > 0)
        {
            signal(SIGTSTP, SIG_IGN);
            printf("\nSIGTSTP sended to [%d]\n", child_pid);
            sleep(1);
            kill(child_pid, SIGTSTP);
        }
        break;
    case SIGQUIT:
        // signal received when the user presses ctrl-|
        if (child_pid > 0)
        {
            signal(SIGQUIT, SIG_IGN);
            printf("\nSIGQUIT sended to [%d]\n", child_pid);
            sleep(1);
            kill(child_pid, SIGQUIT);
        }
        break;
    }
}
