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

/* -------------------------------
    CONSTANTS 
-------------------------------- */
#define DATABASE_DIR      "database"
#define INDEX_FILE        "database/index.txt"
#define TRANSACTION_LOG   "database/transaction.log"

#define ACCOUNT_NUM_LEN   16
#define NAME_LEN          50
#define ID_LEN            20
#define ACCTYPE_LEN       10
#define PIN_LENGTH        5     // 4 digits + '\0'

/* -------------------------------
    STRUCT
-------------------------------- */
typedef struct {
    char accountNumber[ACCOUNT_NUM_LEN];
    char name[NAME_LEN];
    char id[ID_LEN];
    char accountType[ACCTYPE_LEN]; // "Savings" or "Current"
    char pin[PIN_LENGTH];
    double balance;
} BankAccount;

/* -------------------------------
    FUNCTION PROTOTYPES
-------------------------------- */
/* Input helpers */
void clearInputBuffer(void);
bool readLine(char *buffer, size_t size);
void getCurrentDateTime(char *buffer, int size);

/* Logging */
void logTransaction(const char *action,
                    const char *account,
                    double amount,
                    const char *extra);

/* Account index management */
int  countExistingAccounts(void);
int  loadAllAccounts(char accounts[][ACCOUNT_NUM_LEN]);
bool removeFromIndexFile(const char *target);

/* Validation */
bool validatePIN(const char *pin);
bool isPositiveNumber(const char *str);
bool isValidAmount(double amount);

/* Strict validation helpers */
bool validateNameStrict(const char *name);
bool validateIDStrict(const char *id);

/* Account file operations */
int  generateAccountNumber(void);
bool accountExists(const char *accountNumber);
bool saveAccount(const BankAccount *acc);
bool loadAccount(const char *accountNumber, BankAccount *acc);

/* Account selection menu */
int  showAccountSelection(char accounts[][ACCOUNT_NUM_LEN], int count);

/* ==========================================================
   IMPLEMENTATIONS
   ========================================================== */

/* -------------------------------
    Input Helpers
-------------------------------- */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

bool readLine(char *buffer, size_t size) {
    if (!fgets(buffer, (int)size, stdin))
        return false;
    buffer[strcspn(buffer, "\n")] = '\0';
    return true;
}

void getCurrentDateTime(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", info);
}

/* -------------------------------
    Logging
-------------------------------- */
void logTransaction(const char *action,
                    const char *account,
                    double amount,
                    const char *extra)
{
    FILE *fp = fopen(TRANSACTION_LOG, "a");
    if (!fp) return;

    char stamp[50];
    getCurrentDateTime(stamp, sizeof(stamp));

    fprintf(fp, "[%s] %-15s | Acc=%s | Amount=%.2f",
            stamp,
            action ? action : "-",
            account ? account : "-",
            amount);

    if (extra && extra[0] != '\0')
        fprintf(fp, " | Extra=%s", extra);

    fprintf(fp, "\n");
    fclose(fp);
}

/* -------------------------------
    Index File 
-------------------------------- */
int countExistingAccounts(void) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return 0;

    int cnt = 0;
    char acc[ACCOUNT_NUM_LEN];

    while (fscanf(fp, "%s", acc) == 1)
        cnt++;

    fclose(fp);
    return cnt;
}

int loadAllAccounts(char accounts[][ACCOUNT_NUM_LEN]) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return 0;

    int count = 0;
    while (count < 100 && fscanf(fp, "%s", accounts[count]) == 1)
        count++;

    fclose(fp);
    return count;
}

bool removeFromIndexFile(const char *target) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return false;

    FILE *temp = fopen("database/index_tmp.txt", "w");
    if (!temp) {
        fclose(fp);
        return false;
    }

    char acc[ACCOUNT_NUM_LEN];
    bool found = false;

    while (fscanf(fp, "%s", acc) == 1) {
        if (strcmp(acc, target) == 0)
            found = true;
        else
            fprintf(temp, "%s\n", acc);
    }

    fclose(fp);
    fclose(temp);

    remove(INDEX_FILE);
    rename("database/index_tmp.txt", INDEX_FILE);

    return found;
}

/* -------------------------------
    Validation 
-------------------------------- */
bool validatePIN(const char *pin) {
    if (!pin || strlen(pin) != 4)
        return false;
    for (int i = 0; i < 4; i++)
        if (!isdigit((unsigned char)pin[i])) return false;
    return true;
}

bool isPositiveNumber(const char *str) {
    if (!str || !str[0])
        return false;

    int dot = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '.') {
            if (++dot > 1) return false;
        } else if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }

    return true;
}

bool isValidAmount(double amount) {
    return amount >= 0.01 && amount <= 50000.0;
}

/* -------------------------------
   Strict Validation: Name (letters + spaces only)
-------------------------------- */
bool validateNameStrict(const char *name) {
    if (!name) return false;

    size_t len = strlen(name);
    if (len == 0) return false;

    bool hasAlpha = false;

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)name[i];

        if (isalpha(c)) {
            hasAlpha = true;
        } else if (c == ' ') {
            continue;  // space allowed
        } else {
            return false;  // digits or symbols not allowed
        }
    }

    return hasAlpha;  // must have at least one letter
}

/* -------------------------------
    Strict Validation: ID (must be exactly 12 digits)
-------------------------------- */
bool validateIDStrict(const char *id) {
    if (!id) return false;

    size_t len = strlen(id);
    if (len != 12) return false;

    for (size_t i = 0; i < 12; i++) {
        if (!isdigit((unsigned char)id[i]))
            return false;
    }

    return true;
}

/* -------------------------------
    Account Files 
-------------------------------- */
bool accountExists(const char *accountNumber) {
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, accountNumber);

    FILE *fp = fopen(path, "r");
    if (fp) {
        fclose(fp);
        return true;
    }
    return false;
}

int generateAccountNumber(void) {
    for (int attempt = 0; attempt < 1000; attempt++) {

        int digits = 7 + rand() % 3;  // 7–9 digits

        int min = 1;
        for (int i = 1; i < digits; i++)
            min *= 10;

        int max = min * 10 - 1;

        int num = min + rand() % (max - min + 1);

        char buf[ACCOUNT_NUM_LEN];
        snprintf(buf, sizeof(buf), "%d", num);

        if (!accountExists(buf)) {
            FILE *idx = fopen(INDEX_FILE, "a");
            if (!idx) return -1;

            fprintf(idx, "%s\n", buf);
            fclose(idx);

            return num;
        }
    }

    return -1;
}

bool saveAccount(const BankAccount *acc) {
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, acc->accountNumber);

    FILE *fp = fopen(path, "w");
    if (!fp) return false;

    fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%.2f\n",
            acc->accountNumber,
            acc->name,
            acc->id,
            acc->accountType,
            acc->pin,
            acc->balance);

    fclose(fp);
    return true;
}

bool loadAccount(const char *accountNumber, BankAccount *acc) {
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, accountNumber);

    FILE *fp = fopen(path, "r");
    if (!fp) return false;

    fgets(acc->accountNumber, ACCOUNT_NUM_LEN, fp);
    acc->accountNumber[strcspn(acc->accountNumber, "\n")] = '\0';

    fgets(acc->name, NAME_LEN, fp);
    acc->name[strcspn(acc->name, "\n")] = '\0';

    fgets(acc->id, ID_LEN, fp);
    acc->id[strcspn(acc->id, "\n")] = '\0';

    fgets(acc->accountType, ACCTYPE_LEN, fp);
    acc->accountType[strcspn(acc->accountType, "\n")] = '\0';

    fgets(acc->pin, PIN_LENGTH, fp);
    acc->pin[strcspn(acc->pin, "\n")] = '\0';

    fscanf(fp, "%lf", &acc->balance);

    fclose(fp);
    return true;
}

/* -------------------------------
    ACCOUNT SELECTION MENU 
-------------------------------- */
int showAccountSelection(char accounts[][ACCOUNT_NUM_LEN], int count) {
    if (count == 0) {
        printf("No accounts available.\n");
        return -1;
    }

    printf("\n=== Available Accounts ===\n");
    for (int i = 0; i < count; i++) {
        BankAccount acc;
        if (loadAccount(accounts[i], &acc)) {
            printf("%d. %s  (%s)  - %s\n",
                   i + 1,
                   acc.accountNumber,
                   acc.name,
                   acc.accountType);
        }
    }

    char input[10];
    int choice;

    while (1) {
        printf("Choose an account (1-%d): ", count);
        if (!readLine(input, sizeof(input))) return -1;

        if (!isPositiveNumber(input)) {
            printf("Please enter a valid number.\n");
            continue;
        }

        choice = atoi(input);
        if (choice >= 1 && choice <= count)
            return choice - 1;

        printf("Invalid option.\n");
    }
}

#endif
