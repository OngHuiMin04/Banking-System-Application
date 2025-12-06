#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "common.h"

bool depositMoney(void);

/* ==========================================================
   DEPOSIT MONEY FUNCTION
   ========================================================== */
bool depositMoney(void) {

    printf("\n===============================================\n");
    printf("                  DEPOSIT MONEY                \n");
    printf("===============================================\n");

    /* ======================================================
       SELECT ACCOUNT
       ====================================================== */
    char accounts[100][ACCOUNT_NUM_LEN];
    int count = loadAllAccounts(accounts);
    int index = showAccountSelection(accounts, count);
    if (index == -1) return false;

    char accountNumber[ACCOUNT_NUM_LEN];
    strcpy(accountNumber, accounts[index]);

    BankAccount acc;
    if (!loadAccount(accountNumber, &acc)) {
        printf("Error: Account not found.\n");
        return false;
    }

    /* ======================================================
       PIN VERIFICATION
       ====================================================== */
    char pinInput[50];
    printf("\nEnter 4-Digit PIN: ");
    if (!readLine(pinInput, sizeof(pinInput)) || strcmp(pinInput, acc.pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    /* ======================================================
       ENTER DEPOSIT AMOUNT
       ====================================================== */
    char amountStr[32];
    double amount = 0.0;

    while (1) {
        printf("\nEnter Amount to Deposit (RM 0.01 - RM 50000.00): ");

        if (!readLine(amountStr, sizeof(amountStr)))
            return false;

        /* CHECK VALID NUMBER FORMAT */
        bool validFormat = true;
        int dots = 0;

        for (int i = 0; amountStr[i] != '\0'; i++) {
            if (amountStr[i] == '.') {
                dots++;
                if (dots > 1) { validFormat = false; break; }
            } 
            else if (!isdigit((unsigned char)amountStr[i])) {
                validFormat = false;
                break;
            }
        }

        if (!validFormat) {
            printf("Invalid input. Please enter digits only.\n");
            continue;
        }

        amount = atof(amountStr);

        if (amount < 0.01 || amount > 50000.00) {
            printf("Amount must be between RM 0.01 and RM 50000.00.\n");
            continue;
        }

        break;
    }

    /* ======================================================
       UPDATE BALANCE
       ====================================================== */
    acc.balance += amount;
    saveAccount(&acc);

    /* ======================================================
       OUTPUT RESULT
       ====================================================== */
    printf("\nDeposit Successful.\n\n");
    printf("New Balance: RM %.2f\n", acc.balance);

    logTransaction("DEPOSIT", accountNumber, amount, "");

    return true;
}

#endif
