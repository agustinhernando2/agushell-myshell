// include/functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define AMARILLO "\x1b[33m"
#define GRIS "\x1b[37m"
#define CELESTE "\x1b[36m"
#define ROJO "\x1b[31m"
#define CLEAR "\033[H\033[J"

#define CMD_MAX 500
#define DOLLAR "$"
#define SPACE " "
#define PWD "PWD"
#define USER "USER"
#define OLDPWD "OLDPWD"

#include <limits.h>

/**
 * @brief obtiene el valor de la variable running.
 * @return running
 * */
int get_running();

/**
 * @brief actualiza las variables de entorno cuando se cambia de directorio
 * y personaliza el command prompt de myshell
 **/
void show_prompt(char* username, char* hostname);

/**
 * @brief compara el comando y ejecuta funciones que respondan en funcion de él.
 **/
void exec_command(char* command);

/**
 * @brief Limpia el command line de myshell.
 **/
void clear_shell();

/**
 * @brief exit.
 **/
void exit_shell();

/**
 * @brief solicita el directorio actual.
 * @return currentDirectory
 **/
char* get_directory(char* command);

/**
 * @brief modifica el directorio actual en la variable de entorno.
 **/
void exchange_directory(char* dir);

/**
 * @brief ejecuta echo.
 **/
void echo_shell(char* msg, size_t len);

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
