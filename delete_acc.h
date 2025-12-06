#ifndef DELETE_ACCOUNT_H
#define DELETE_ACCOUNT_H

#include "common.h"

bool deleteBankAccount(void);

bool deleteBankAccount(void) {
    printf("\n=== Delete Bank Account ===\n");

    char accounts[100][ACCOUNT_NUM_LEN];
    int count = loadAllAccounts(accounts);
    if (count == 0) {
        printf("No accounts available.\n");
        return false;
    }

    int index = showAccountSelection(accounts, count);
    if (index == -1) return false;

    char targetAccount[ACCOUNT_NUM_LEN];
    strcpy(targetAccount, accounts[index]);

    // Confirm re-entry
    char confirm[ACCOUNT_NUM_LEN];
    printf("Re-enter the account number to confirm: ");
    if (!readLine(confirm, sizeof(confirm))) return false;

    if (strcmp(confirm, targetAccount) != 0) {
        printf("Mismatch. Cancelled.\n");
        return false;
    }

    // Load account
    BankAccount acc;
    if (!loadAccount(targetAccount, &acc)) {
        printf("Cannot load account.\n");
        return false;
    }

    // Delete file
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, targetAccount);
    remove(path);

    // Remove from index
    removeFromIndexFile(targetAccount);

    printf("Account deleted.\n");
    logTransaction("DELETE_ACCOUNT", targetAccount, 0.0, "");

    return true;
}

#endif
