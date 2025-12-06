#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include "common.h"

void createNewAccount(void);

/* ==========================================================
   CREATE NEW ACCOUNT
   ========================================================== */
void createNewAccount(void) {
    BankAccount new_account;
    char buffer[100];

    printf("\n===============================================\n");
    printf("              CREATE NEW ACCOUNT\n");
    printf("===============================================\n");

    /* ======================================================
       ENTER NAME (LETTERS + SPACES ONLY)
       ====================================================== */
    while (1) {
        printf("\nEnter Account Holder Name: ");
        if (!readLine(new_account.name, sizeof(new_account.name)))
            return;

        if (!validateNameStrict(new_account.name)) {
            printf("  Invalid name. Only alphabets and spaces allowed.\n");
            continue;
        }
        break;
    }

    /* ======================================================
       ENTER IDENTIFICATION NUMBER (12 DIGITS ONLY)
       ====================================================== */
    while (1) {
        printf("\nEnter Identification Number (12 digits): ");
        if (!readLine(new_account.id, sizeof(new_account.id)))
            return;

        if (!validateIDStrict(new_account.id)) {
            printf("  Invalid ID. Your ID must contain exactly 12 digits.\n");
            continue;
        }
        break;
    }

    /* ======================================================
       SELECT ACCOUNT TYPE
       ====================================================== */
    printf("\nSelect Account Type:\n");
    printf("  1. Savings Account\n");
    printf("  2. Current Account\n");

    while (1) {
        printf("Choose Option (1 or 2): ");
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
            printf("  Invalid option. Please enter 1 or 2.\n");
        }
    }

    /* ======================================================
       ENTER 4-DIGIT PIN
       ====================================================== */
    while (1) {
        char pinInput[50];

        printf("\nEnter 4-Digit PIN: ");
        if (!readLine(pinInput, sizeof(pinInput)))
            return;

        if (strlen(pinInput) != 4) {
            printf("  Invalid PIN. PIN must be exactly 4 digits.\n");
            continue;
        }

        bool allDigits = true;
        for (int i = 0; i < 4; i++) {
            if (!isdigit((unsigned char)pinInput[i])) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            printf("  Invalid PIN. PIN must contain digits only.\n");
            continue;
        }

        strcpy(new_account.pin, pinInput);
        break;
    }

    /* ======================================================
       GENERATE ACCOUNT NUMBER
       ====================================================== */
    int account_num = generateAccountNumber();
    if (account_num == -1) {
        printf("\nERROR: Failed to generate unique account number.\n");
        return;
    }

    snprintf(new_account.accountNumber,
             sizeof(new_account.accountNumber),
             "%d",
             account_num);

    new_account.balance = 0.0;

    /* ======================================================
       SAVE ACCOUNT TO FILE
       ====================================================== */
    if (!saveAccount(&new_account)) {
        printf("\nERROR: Failed to save account.\n");
        return;
    }

    /* ======================================================
       DISPLAY SUCCESS CONFIRMATION
       ====================================================== */
    printf("\n===============================================\n");
    printf("           ACCOUNT CREATED SUCCESSFULLY\n");
    printf("===============================================\n\n");

    printf("Account Number : %s\n", new_account.accountNumber);
    printf("Name           : %s\n", new_account.name);
    printf("Account Type   : %s\n", new_account.accountType);
    printf("Initial Balance: RM %.2f\n", new_account.balance);

    logTransaction("CREATE_ACCOUNT", new_account.accountNumber, 0.0, "");
}

#endif
