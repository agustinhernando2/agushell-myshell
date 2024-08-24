// include/common.h
#ifndef COMMON_H
#define COMMON_H

#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define YELLOW "\x1b[33m"
#define GRAY "\x1b[37m"
#define BLUE "\x1b[36m"
#define RED "\x1b[31m"
#define CLEAR "\033[H\033[J"
#define NORMAL "\033[0m"

#define PATH_MAX 4096 /* # chars in a path name including nul */
#define MAX_LENGHT 301
#define CMD_MAX 500
#define BUFFER_SIZE 1024

#define MAX_LINE 1024
#define MAX_ARGS 64

#define DOLLAR "$"
#define SPACE " "
#define PWD "PWD"
#define HOME "HOME"
#define USER "USER"
#define OLDPWD "OLDPWD"

#include "../include/advanced.h"
#include "../include/external.h"
#include "../include/internal.h"

/**
 * @brief execute command in myshell.
 **/
int exec_command(char* command);

/**
 * @brief remove the beginning of a string.
 * @param str1 string to remove the beginning.
 **/
void remove_beginning_spaces(char* str1);

/**
 * @brief remove the spaces of a string.
 * @param str1 string to remove the spaces.
 **/
void remove_spaces(char* str1);

/**
 * @brief remove a character from a string.
 * @param str string to remove the character.
 * @param charToRemmove character to remove.
 **/
void remove_char(char* str, char charToRemmove);

#endif // COMMON_H
