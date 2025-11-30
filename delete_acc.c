#include "delete_acc.h"

bool deleteBankAccount(void) {
    printf("\n=== Delete Bank Account ===\n");

    // Load all accounts
    char accounts[100][ACCOUNT_NUM_LEN];
    int count = loadAllAccounts(accounts);
    if (count == 0) {
        printf("No accounts available.\n");
        return false;
    }

    // Display accounts
    printf("\nExisting Accounts:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, accounts[i]);
    }

    // Select account
    char choiceStr[16];
    int choice;
    while (1) {
        printf("Select account number to delete (1-%d): ", count);
        if (!readLine(choiceStr, sizeof(choiceStr))) return false;
        if (!isPositiveNumber(choiceStr)) continue;
        choice = atoi(choiceStr);
        if (choice >= 1 && choice <= count) break;
        printf("Invalid selection.\n");
    }

    // Target
    char targetAccount[ACCOUNT_NUM_LEN];
    strcpy(targetAccount, accounts[choice - 1]);

    // Re-enter confirmation
    char confirmAcc[ACCOUNT_NUM_LEN];
    printf("Re-enter the account number to confirm: ");
    if (!readLine(confirmAcc, sizeof(confirmAcc))) return false;
    if (strcmp(confirmAcc, targetAccount) != 0) {
        printf("Account number mismatch. Cancelled.\n");
        return false;
    }

    // Load account file
    BankAccount acc;
    if (!loadAccount(targetAccount, &acc)) {
        printf("Failed to load account file.\n");
        return false;
    }

    // Verify last 4 ID
    char last4[5];
    size_t len = strlen(acc.id);
    strncpy(last4, acc.id + len - 4, 4);
    last4[4] = '\0';

    char inputLast4[5];
    printf("Enter last 4 characters of ID: ");
    if (!readLine(inputLast4, sizeof(inputLast4))) return false;

    // PIN check
    char pin[PIN_LENGTH];
    printf("Enter 4-digit PIN to confirm deletion: ");
    if (!readLine(pin, sizeof(pin)) || !validatePIN(pin)) {
        printf("Invalid PIN.\n");
        return false;
    }

    if (strcmp(pin, acc.pin) != 0 || strcmp(inputLast4, last4) != 0) {
        printf("Authentication failed.\n");
        return false;
    }

    // Final confirmation
    printf("Are you sure you want to delete account %s? (y/n): ", targetAccount);
    if (!readLine(choiceStr, sizeof(choiceStr))) return false;
    if (choiceStr[0] != 'y' && choiceStr[0] != 'Y') {
        printf("Cancelled.\n");
        return false;
    }

    // Delete account file
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, targetAccount);
    remove(path);

    // Remove from index
    removeFromIndexFile(targetAccount);

    printf("Account deleted successfully.\n");
    logTransaction("DELETE_ACCOUNT", targetAccount, 0.0, "");

    return true;
}