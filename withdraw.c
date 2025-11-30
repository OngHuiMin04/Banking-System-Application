#include "withdraw.h"

bool withdrawMoney(void) {
    printf("\n=== Withdraw Money ===\n");

    char accountNumber[ACCOUNT_NUM_LEN];
    char pin[PIN_LENGTH];
    char amountStr[32];
    double amount;

    // Get account number
    printf("Enter account number: ");
    if (!readLine(accountNumber, sizeof(accountNumber)) || strlen(accountNumber) == 0) {
        printf("Invalid account number.\n");
        return false;
    }

    // Get PIN
    printf("Enter 4-digit PIN: ");
    if (!readLine(pin, sizeof(pin)) || !validatePIN(pin)) {
        printf("Invalid PIN format.\n");
        return false;
    }

    // Load account
    BankAccount acc;
    if (!loadAccount(accountNumber, &acc)) {
        printf("Account not found.\n");
        return false;
    }

    // PIN check
    if (strcmp(acc.pin, pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    printf("Current balance: RM %.2f\n", acc.balance);

    // Get amount
    while (1) {
        printf("Enter amount to withdraw: ");
        if (!readLine(amountStr, sizeof(amountStr))) return false;

        if (!isPositiveNumber(amountStr)) {
            printf("Please enter numbers only.\n");
            continue;
        }

        amount = atof(amountStr);

        if (!isValidAmount(amount)) {
            printf("Amount must be RM 0.01 - RM 50,000.\n");
            continue;
        }

        if (amount > acc.balance) {
            printf("Insufficient funds (Balance: RM %.2f).\n", acc.balance);
            return false;
        }
        break;
    }

    // Deduct amount
    acc.balance -= amount;

    // Save file
    if (!saveAccount(&acc)) {
        printf("Failed to update account file.\n");
        return false;
    }

    printf("Withdrawal successful! New balance: RM %.2f\n", acc.balance);
    logTransaction("WITHDRAWAL", acc.accountNumber, amount, "");

    return true;
}
