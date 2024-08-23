// include/functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define YELLOW "\x1b[33m"
#define GRAY "\x1b[37m"
#define BLUE "\x1b[36m"
#define RED "\x1b[31m"
#define CLEAR "\033[H\033[J"
#define NORMAL "\033[0m"

#define PATH_MAX 4096 /* # chars in a path name including nul */
#define CMD_MAX 500
#define BUFFER_SIZE 1024
#define DOLLAR "$"
#define SPACE " "
#define PWD "PWD"
#define USER "USER"
#define OLDPWD "OLDPWD"

/**
 * @brief get running status.
 * @return running
 * */
int get_running();

/**
 * @brief generate prompt.
 **/
char* generate_prompt(char* username, char* hostname);

/**
 * @brief execute command in myshell.
 **/
int exec_command(char* command);

/**
 * @brief clear shell.
 * @return 0 if success.
 * @return -1 if error.
 **/
int clear_shell();

/**
 * @brief exit.
 **/
void exit_shell();

/**
 * @brief modify the current directory.
 * @param command directory to change or command.
 * @return 0 if success.
 * @return -1 if error.
 **/
int exchange_directory(char* command);

/**
 * @brief execute echo command. This function will print the message or comment.
 * @param msg message to print.
 * @param msglen length of the message.
 * @param buffer buffer to store the message.
 * @param bufflen length of the buffer.
 * @return
 *      0 if success
 * ,    -1 Environment not found
 * ,    -2 Error en snprintf
 * ,    -3 if msg is NULL
 * ,    -4 if bufflen is 0.
 **/
int echo_shell(char* msg, size_t msglen, char* buffer, size_t bufflen);

/**
 * @brief solicitar un comando ingresado por el usuario el cual se guardará en una variable command.
 **/
void get_command(char* command);

/**
 * @brief obtiene los argumentos para el comando externo.
 * @return argumentos
 * */
char** get_arguments_for_extern_command(char* command);

/**
 * @brief ejecuta el comando en caso con system como ultima alternativa.
 **/
void extern_command(char* c[]);

#endif // FUNCTIONS_H
