// include/external.h
#ifndef EXTERNAL_H
#define EXTERNAL_H

#include "common.h"

/**
 * @brief ejecuta el comando en caso con system como ultima alternativa.
 **/
void extern_command(char* input);

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

void process_command(char* input, int8_t is_background);
void execute_binary(char* program, int8_t is_background);
void set_redirect(char* redirectin, char* c, FILE* stream);

void handle_redirections(char* token, char** redirectin, char** redirectout);
void exec(char* redirectin, char* redirectout, int8_t is_background, char* input);
#endif // EXTERNAL_H
