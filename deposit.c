#include "deposit.h"

bool depositMoney(void) {
    printf("\n=== Deposit Money ===\n");

    char accountNumber[ACCOUNT_NUM_LEN];
    char pin[PIN_LENGTH];
    char amountStr[32];
    double amount;

    printf("Enter account number: ");
    if (!readLine(accountNumber, sizeof(accountNumber))) return false;

    printf("Enter 4-digit PIN: ");
    if (!readLine(pin, sizeof(pin)) || !validatePIN(pin)) return false;

    // Load
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

    // Amount
    while (1) {
        printf("Enter deposit amount: ");
        if (!readLine(amountStr, sizeof(amountStr))) return false;
        if (!isPositiveNumber(amountStr)) continue;
        amount = atof(amountStr);
        if (isValidAmount(amount)) break;
    }

    acc.balance += amount;

    // Save
    saveAccount(&acc);

    printf("Deposit successful. New balance: RM %.2f\n", acc.balance);
    logTransaction("DEPOSIT", accountNumber, amount, "");

    return true;
}