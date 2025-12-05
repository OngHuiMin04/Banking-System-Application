#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include "common.h"

/* ------------------------------------------
   Function Prototype
------------------------------------------- */
void createNewAccount(void);

/* ------------------------------------------
   Full Function Implementation
------------------------------------------- */
void createNewAccount(void) {
    BankAccount new_account;
    char buffer[100];

    printf("\n=== Create New Account ===\n");

    // Name
    printf("Enter account holder name: ");
    if (!readLine(new_account.name, sizeof(new_account.name)) ||
        !validateName(new_account.name)) {
        printf("Invalid name.\n");
        return;
    }

    // ID
    printf("Enter identification number (ID): ");
    if (!readLine(new_account.id, sizeof(new_account.id)) ||
        !validateID(new_account.id)) {
        printf("Invalid ID.\n");
        return;
    }

    // Account type
    printf("Enter account type (Savings/Current): ");
    if (!readLine(buffer, sizeof(buffer)) ||
        !validateAccountType(buffer, new_account.accountType)) {
        printf("Invalid account type. Please enter 'Savings' or 'Current'.\n");
        return;
    }

    // PIN
    printf("Enter 4-digit PIN: ");
    if (!readLine(new_account.pin, sizeof(new_account.pin)) ||
        !validatePIN(new_account.pin)) {
        printf("Invalid PIN. Must be exactly 4 digits.\n");
        return;
    }

    // Generate account number
    int account_num = generateAccountNumber();
    if (account_num == -1) {
        printf("Failed to generate unique account number.\n");
        return;
    }
    snprintf(new_account.accountNumber,
             sizeof(new_account.accountNumber), "%d", account_num);

    new_account.balance = 0.0;

    // Save file
    if (!saveAccount(&new_account)) {
        printf("Error saving account file.\n");
        return;
    }

    printf("\nAccount created successfully!\n");
    printf("Account Number : %s\n", new_account.accountNumber);
    printf("Name           : %s\n", new_account.name);
    printf("Account Type   : %s\n", new_account.accountType);
    printf("Initial Balance: RM %.2f\n", new_account.balance);

    logTransaction("CREATE_ACCOUNT", new_account.accountNumber, 0.0, "");
}

#endif
