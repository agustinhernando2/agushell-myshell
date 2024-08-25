// include/external.h
#ifndef EXTERNAL_H
#define EXTERNAL_H

#include "common.h"

/**
 * @brief obtiene los argumentos para el comando externo.
 * @return argumentos
 * */
char** get_arguments_for_extern_command(char* command);

/**
 * @brief ejecuta el comando en caso con system como ultima alternativa.
 **/
void extern_command(char* input);

void execute_binary(char* input, char* redirectin, char* redirectout);

void execute_pipes(char input[], char* redirectin, char* redirectout);

/**
 * @brief Handles signals received by the program.
 *
 * This function is responsible for handling signals such as SIGCHLD, SIGINT, SIGTSTP, and SIGQUIT.
 * It performs different actions based on the signal received.
 *
 * @param signum The signal number received.
 */
void handler(int signum);

/**
 * @brief parse the input.
 * Make an array of strings from the input to be used as params in execvp.
 * Args is an array of strings.
 * The first element is the command and the rest are the arguments.
 * The last element is NULL.
 */
void parse_input(char* input, char** args);

char* parse_token(char* input, const char* delimiter);
void execute_command(char* program, char* redirectin, char* redirectout);
void handle_redirections(char* token, char** redirectin, char** redirectout);
#endif // EXTERNAL_H
