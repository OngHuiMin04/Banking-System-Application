#ifndef WITHDRAW_H
#define WITHDRAW_H

#include "common.h"

bool withdrawMoney(void);

bool withdrawMoney(void) {
    printf("\n========== WITHDRAW MONEY ==========\n");

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
       SHOW CURRENT BALANCE
    ----------------------------- */
    printf("\nCurrent Balance: RM %.2f\n", acc.balance);

    /* -----------------------------
       ENTER WITHDRAWAL AMOUNT
    ----------------------------- */
    char amountStr[32];
    double amount = 0.0;

    while (1) {
        printf("\nEnter amount to withdraw (RM 0.01 - RM 50000.00): ");

        if (!readLine(amountStr, sizeof(amountStr)))
            return false;

        // Validate numeric format
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

        amount = atof(amountStr);

        if (amount < 0.01 || amount > 50000.00) {
            printf("Amount must be between RM 0.01 and RM 50000.00.\n");
            continue;
        }

        if (amount > acc.balance) {
            printf("Insufficient funds. Your balance is RM %.2f.\n", acc.balance);
            continue;
        }

        break;
    }

    /* -----------------------------
       UPDATE BALANCE
    ----------------------------- */
    acc.balance -= amount;
    saveAccount(&acc);

    /* -----------------------------
       OUTPUT RESULT
    ----------------------------- */
    printf("\nWithdrawal successful!\n\n");
    printf("New Balance: RM %.2f\n", acc.balance);

    logTransaction("WITHDRAW", accountNumber, amount, "");

    return true;
}

#endif
