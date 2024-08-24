// include/internal.h
#ifndef INTERNAL_H
#define INTERNAL_H

#include "common.h"

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

#endif // INTERNAL_H
