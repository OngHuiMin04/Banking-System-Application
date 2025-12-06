#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "common.h"

bool depositMoney(void);

bool depositMoney(void) {
    printf("\n========== DEPOSIT MONEY ==========\n");

    /* -----------------------------
       SELECT ACCOUNT
    ----------------------------- */
    char accounts[100][ACCOUNT_NUM_LEN];
    int count = loadAllAccounts(accounts);
    int index = showAccountSelection(accounts, count);
    if (index == -1) return false;

    char accountNumber[ACCOUNT_NUM_LEN];
    strcpy(accountNumber, accounts[index]);

    BankAccount acc;
    if (!loadAccount(accountNumber, &acc)) {
        printf("Account not found.\n");
        return false;
    }

    /* -----------------------------
       PIN VERIFICATION
    ----------------------------- */
    char pinInput[50];
    printf("Enter 4-digit PIN: ");
    if (!readLine(pinInput, sizeof(pinInput)) || strcmp(pinInput, acc.pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    /* -----------------------------
       ENTER DEPOSIT AMOUNT
    ----------------------------- */
    char amountStr[32];
    double amount = 0.0;

    while (1) {
        printf("\nEnter amount to deposit (RM 0.01 - RM 50000.00): ");

        if (!readLine(amountStr, sizeof(amountStr)))
            return false;

        // Check numeric format manually
        bool validFormat = true;
        int dots = 0;

        for (int i = 0; amountStr[i] != '\0'; i++) {
            if (amountStr[i] == '.') {
                dots++;
                if (dots > 1) { validFormat = false; break; }
            } else if (!isdigit((unsigned char)amountStr[i])) {
                validFormat = false;
                break;
            }
        }

        if (!validFormat) {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }

        // Convert to double
        amount = atof(amountStr);

        if (amount < 0.01 || amount > 50000.00) {
            printf("Amount must be between RM 0.01 and RM 50000.00.\n");
            continue;
        }

        break;  // Valid amount
    }

    /* -----------------------------
       UPDATE BALANCE
    ----------------------------- */
    acc.balance += amount;
    saveAccount(&acc);

    /* -----------------------------
       OUTPUT RESULT
    ----------------------------- */
    printf("\nDeposit successful!\n\n");
    printf("New Balance: RM %.2f\n", acc.balance);

    logTransaction("DEPOSIT", accountNumber, amount, "");

    return true;
}

#endif
