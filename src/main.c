#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/common.h"

void print_banner()
{
    printf("    _______  ________ __  _______  ________ ____\n");
    printf("   / ___/ / / / __/ // / / ___/ / / / __/ // / /\n");
    printf("  / (_ / /_/ /\\ \\/ _  / / (_ / /_/ /\\ \\/ _  /_/ \n");
    printf("  \\___/\\____/___/_//_/  \\___/\\____/___/_//_(_)  \n");
    printf("\n");
}

int main(int argc, char* argv[])
{

    // default signal handling
    signal(SIGCHLD, handler);
    signal(SIGTSTP, handler);
    signal(SIGQUIT, handler);
    signal(SIGINT, handler);

    char* username = getenv(USER);
    if (username == NULL)
    {
        fprintf(stderr, "Error: USER environment variable not set\n");
        exit(EXIT_FAILURE);
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    char command[CMD_MAX];
    print_banner();
    if (argc == 1)
    {
        while (get_running())
        {
            char* prompt = generate_prompt(username, hostname);
            if (prompt == NULL)
            {
                fprintf(stderr, "Error: prompt not generated\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("%s", prompt);
            };

            get_command(command);
            
            // if the command is empty, continue and ask for another command
            if(strcmp("\n",command) == 0 || strcmp("",command) == 0 || strlen(command) == 0){
                continue;
            }

            int r = exec_command(command);
            if (r == -1)
            {
                fprintf(stderr, "Error: exec_command\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    exit(EXIT_SUCCESS);
}
