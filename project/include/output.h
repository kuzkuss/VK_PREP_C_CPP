#ifndef PROJECT_INCLUDE_OUTPUT_H_
#define PROJECT_INCLUDE_OUTPUT_H_

#include "utils.h"
#include <stdio.h>

void print_info_client(void);
void print_info_transaction(void);
void print_menu(void);
void print_to_file(FILE *f, const Data *client_data);

#endif  // PROJECT_INCLUDE_OUTPUT_H_
