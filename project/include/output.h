#ifndef PROJECT_INCLUDE_OUTPUT_H_
#define PROJECT_INCLUDE_OUTPUT_H_

#include "utils.h"
#include <stdio.h>

void print_info_client();
void print_info_transaction();
void print_menu();
void print_to_file(FILE *f, Data client_data);

#endif  // PROJECT_INCLUDE_OUTPUT_H_
