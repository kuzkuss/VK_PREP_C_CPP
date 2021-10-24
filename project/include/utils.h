#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#define filename_transaction "transaction.dat"
#define filename_record "record.dat"
#define filename_blackrecord "blackrecord.dat"
#define SIZE_NAME 20
#define SIZE_SURNAME 20
#define SIZE_ADDRESS 30
#define SIZE_TEL 15

struct masterRecord {
    int Number;
    char Name[SIZE_NAME];
    char Surname[SIZE_SURNAME];
    char address[SIZE_ADDRESS];
    char TelNumber[SIZE_TEL];
    double indebtedness;
    double credit_limit;
    double cash_payments;
};
typedef struct masterRecord Data;

#endif  // PROJECT_INCLUDE_UTILS_H_
