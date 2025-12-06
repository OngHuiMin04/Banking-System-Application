#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include "common.h"

void createNewAccount(void);

void createNewAccount(void) {
    BankAccount new_account;
    char buffer[100];

    printf("\n=== Create New Account ===\n");

    /* -----------------------------
       ENTER NAME (letters only)
    ----------------------------- */
    while (1) {
        printf("Enter account holder name: ");
        if (!readLine(new_account.name, sizeof(new_account.name)))
            return;

        if (!validateNameStrict(new_account.name)) {
            printf("Invalid name. Only alphabets and spaces allowed.\n");
            continue;  // ask again
        }

        break;
    }

    /* -----------------------------
       ENTER ID (12 digits)
    ----------------------------- */
    while (1) {
        printf("Enter identification number (12 digits): ");
        if (!readLine(new_account.id, sizeof(new_account.id)))
            return;

        if (!validateIDStrict(new_account.id)) {
            printf("ID must contain exactly 12 digits.\n");
            continue;  // ask again
        }

        break;
    }

    /* -----------------------------
       ACCOUNT TYPE
    ----------------------------- */
    printf("\nSelect Account Type:\n");
    printf("1. Savings\n");
    printf("2. Current\n");

    while (1) {
        printf("Choose option: ");
        if (!readLine(buffer, sizeof(buffer))) return;

        if (strcmp(buffer, "1") == 0) {
            strcpy(new_account.accountType, "Savings");
            break;
        }
        else if (strcmp(buffer, "2") == 0) {
            strcpy(new_account.accountType, "Current");
            break;
        }
        else {
            printf("Invalid option. Please enter 1 or 2.\n");
        }
    }

    /* -----------------------------
       ENTER PIN (4 digits)
    ----------------------------- */
    while (1) {
    char pinInput[50];  // <--- Bigger temporary buffer

    printf("Enter 4-digit PIN: ");
    if (!readLine(pinInput, sizeof(pinInput)))
        return;

    // Check length EXACTLY 4
    if (strlen(pinInput) != 4) {
        printf("Invalid PIN. PIN must be exactly 4 digits.\n");
        continue;
    }

    // Check every character is a digit
    bool allDigits = true;
    for (int i = 0; i < 4; i++) {
        if (!isdigit((unsigned char)pinInput[i])) {
            allDigits = false;
            break;
        }
    }

    if (!allDigits) {
        printf("Invalid PIN. Digits only.\n");
        continue;
    }

    // PASS → copy into struct
    strcpy(new_account.pin, pinInput);
    break;
  }

    /* -----------------------------
       GENERATE ACCOUNT NUMBER
    ----------------------------- */
    int account_num = generateAccountNumber();
    if (account_num == -1) {
        printf("Failed to generate unique account number.\n");
        return;
    }
    snprintf(new_account.accountNumber,
             sizeof(new_account.accountNumber), "%d", account_num);

    new_account.balance = 0.0;

    /* -----------------------------
       SAVE ACCOUNT
    ----------------------------- */
    if (!saveAccount(&new_account)) {
        printf("Error saving account.\n");
        return;
    }

    /* -----------------------------
       PRINT SUCCESS MESSAGE
    ----------------------------- */
    printf("\nAccount created successfully!\n\n");  // one blank line after

    printf("Account Number : %s\n", new_account.accountNumber);
    printf("Name           : %s\n", new_account.name);
    printf("Account Type   : %s\n", new_account.accountType);
    printf("Initial Balance: RM %.2f\n", new_account.balance);

    logTransaction("CREATE_ACCOUNT", new_account.accountNumber, 0.0, "");
}

#endif
