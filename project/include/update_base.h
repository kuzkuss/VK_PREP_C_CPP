#ifndef PROJECT_INCLUDE_UPDATE_BASE_H_
#define PROJECT_INCLUDE_UPDATE_BASE_H_

#include <stdio.h>
#include "utils.h"

void update_info(FILE *f_clients, FILE *f_transactions, FILE *blackrecord, Data client_data, Data transfer);

#endif  // PROJECT_INCLUDE_UPDATE_BASE_H_
