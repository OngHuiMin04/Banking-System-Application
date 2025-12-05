#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "common.h"

/* ---------------------------------------
   Function Prototype
---------------------------------------- */
bool depositMoney(void);

/* ---------------------------------------
   Full Function Implementation
---------------------------------------- */
bool depositMoney(void) {
    printf("\n=== Deposit Money ===\n");

    char accountNumber[ACCOUNT_NUM_LEN];
    char pin[PIN_LENGTH];
    char amountStr[32];
    double amount;

    // Account number
    printf("Enter account number: ");
    if (!readLine(accountNumber, sizeof(accountNumber)))
        return false;

    // PIN
    printf("Enter 4-digit PIN: ");
    if (!readLine(pin, sizeof(pin)) || !validatePIN(pin))
        return false;

    // Load account
    BankAccount acc;
    if (!loadAccount(accountNumber, &acc)) {
        printf("Account not found.\n");
        return false;
    }

    // PIN check
    if (strcmp(pin, acc.pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    // Deposit amount
    while (1) {
        printf("Enter deposit amount: ");
        if (!readLine(amountStr, sizeof(amountStr)))
            return false;

        if (!isPositiveNumber(amountStr))
            continue;

        amount = atof(amountStr);

        if (isValidAmount(amount))
            break;
    }

    // Update balance
    acc.balance += amount;

    // Save account
    saveAccount(&acc);

    printf("Deposit successful. New balance: RM %.2f\n", acc.balance);

    // Logging
    logTransaction("DEPOSIT", accountNumber, amount, "");

    return true;
}

#endif
