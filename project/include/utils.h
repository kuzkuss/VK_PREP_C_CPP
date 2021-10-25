#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#define FILENAME_TRANSACTION "transaction.dat"
#define FILENAME_RECORD "record.dat"
#define FILENAME_BLACKRECORD "blackrecord.dat"
#define SIZE_NAME 20
#define SIZE_SURNAME 20
#define SIZE_ADDRESS 30
#define SIZE_TEL 15

struct client {
    int number;
    char name[SIZE_NAME];
    char surname[SIZE_SURNAME];
    char address[SIZE_ADDRESS];
    char tel_number[SIZE_TEL];
    double indebtedness;
    double credit_limit;
    double cash_payments;
};
typedef struct client Data;

#endif  // PROJECT_INCLUDE_UTILS_H_
