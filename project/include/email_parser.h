#ifndef PROJECT_INCLUDE_EMAIL_PARSER_H_
#define PROJECT_INCLUDE_EMAIL_PARSER_H_

#include <stdio.h>

#define ERROR -1
#define OK 0

#define TRUE 1
#define FALSE 0

#define BUF_SIZE 128

int parse(FILE *f, char **from, char **to, char **date, int *parts);

#endif  // EMAIL_PARSER_H
