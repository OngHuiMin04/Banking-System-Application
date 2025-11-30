// common.h
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

#define DATABASE_DIR      "database"
#define INDEX_FILE        "database/index.txt"
#define TRANSACTION_LOG   "database/transaction.log"

#define ACCOUNT_NUM_LEN   16         // enough for 7–9 digits + '\0'
#define NAME_LEN          50
#define ID_LEN            20
#define ACCTYPE_LEN       10
#define PIN_LENGTH        5          // 4 digits + '\0'

typedef struct {
    char accountNumber[ACCOUNT_NUM_LEN];
    char name[NAME_LEN];
    char id[ID_LEN];
    char accountType[ACCTYPE_LEN];   // "Savings" or "Current"
    char pin[PIN_LENGTH];            // "1234"
    double balance;
} BankAccount;

// ===== Global session / login state =====
extern bool isLoggedIn;
extern BankAccount currentUser;

// ===== Utility functions =====
void clearInputBuffer(void);
bool readLine(char *buffer, size_t size);
void getCurrentDateTime(char *buffer, int size);

// logging
void logTransaction(const char *action,
                    const char *account,
                    double amount,
                    const char *extra);

// index / account counting
int  countExistingAccounts(void);
int  loadAllAccounts(char accounts[][ACCOUNT_NUM_LEN]);
bool removeFromIndexFile(const char *target);

// validation
bool validateName(const char *name);
bool validateID(const char *id);
bool validateAccountType(const char *input, char *outType);
bool validatePIN(const char *pin);
bool isPositiveNumber(const char *str);
bool isValidAmount(double amount);

// account file handling
int  generateAccountNumber(void);
bool accountExists(const char *accountNumber);
bool saveAccount(const BankAccount *acc);
bool loadAccount(const char *accountNumber, BankAccount *acc);
bool authenticateAccount(const char *accountNumber, const char *pin);

#endif