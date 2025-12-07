#ifndef DELETE_ACCOUNT_H
#define DELETE_ACCOUNT_H

#include "common.h"

bool deleteBankAccount(void) {

    printf("\n===============================================\n");
    printf("               DELETE BANK ACCOUNT             \n");
    printf("===============================================\n");

    /* ======================================================
       LOAD ALL ACCOUNTS
       ====================================================== */
    char accounts[100][ACCOUNT_NUM_LEN];
    int count = loadAllAccounts(accounts);

    if (count == 0) {
        printf("No accounts available.\n");
        return false;
    }

    /* ======================================================
       SELECT ACCOUNT TO DELETE
       ====================================================== */
    int index = showAccountSelection(accounts, count);
    if (index == -1) return false;

    char targetAccount[ACCOUNT_NUM_LEN];
    strcpy(targetAccount, accounts[index]);

    BankAccount acc;
    if (!loadAccount(targetAccount, &acc)) {
        printf("Error loading account.\n");
        return false;
    }

    printf("\nYou selected account: %s (%s)\n", acc.accountNumber, acc.name);

    /* ======================================================
       STEP 1 — CONFIRM ACCOUNT NUMBER
       ====================================================== */
    char confirmAcc[ACCOUNT_NUM_LEN];
    printf("\nRe-enter ACCOUNT NUMBER to confirm: ");
    if (!readLine(confirmAcc, sizeof(confirmAcc))) return false;

    if (strcmp(confirmAcc, targetAccount) != 0) {
        printf("Account number verification failed.\n");
        return false;
    }

    /* ======================================================
       STEP 2 — CONFIRM LAST 4 DIGITS OF ID
       ====================================================== */
    char idConfirm[10];
    int idLen = strlen(acc.id);

    printf("Enter LAST 4 digits of ID: ");
    if (!readLine(idConfirm, sizeof(idConfirm))) return false;

    if (strlen(idConfirm) != 4 ||
        strncmp(acc.id + idLen - 4, idConfirm, 4) != 0) {
        printf("ID verification failed.\n");
        return false;
    }

    /* ======================================================
       STEP 3 — CONFIRM PIN
       ====================================================== */
    char pinConfirm[PIN_LENGTH];
    printf("Enter 4-digit PIN: ");
    if (!readLine(pinConfirm, sizeof(pinConfirm))) return false;

    if (strcmp(pinConfirm, acc.pin) != 0) {
        printf("PIN verification failed.\n");
        return false;
    }

    /* ======================================================
       FINAL CONFIRMATION LOOP
       ====================================================== */
    char finalConfirm[10];

    while (1) {
        printf("\n-----------------------------------------------\n");
        printf("CONFIRM DELETE ACCOUNT: %s (%s)\n", acc.accountNumber, acc.name);
        printf("This action CANNOT be undone.\n");
        printf("-----------------------------------------------\n");
        printf("Type 'yes' to DELETE or 'no' to CANCEL: ");

        if (!readLine(finalConfirm, sizeof(finalConfirm)))
            return false;

        /* convert input to lowercase */
        for (int i = 0; finalConfirm[i]; i++)
            finalConfirm[i] = (char)tolower((unsigned char)finalConfirm[i]);

        if (strcmp(finalConfirm, "yes") == 0) {
            break;  // valid → proceed to deletion
        } 
        else if (strcmp(finalConfirm, "no") == 0) {
            printf("\nDeletion cancelled. Returning to menu...\n");
            return false;  // valid → cancel
        } 
        else {
            printf("\nInvalid input. Please type only 'yes' or 'no'.\n");
        }
    }

    /* ======================================================
       DELETE ACCOUNT FILE
       ====================================================== */
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, targetAccount);

    if (remove(path) != 0) {
        printf("Error: Unable to delete account file.\n");
        return false;
    }

    removeFromIndexFile(targetAccount);

    printf("\n===============================================\n");
    printf("      ACCOUNT %s DELETED SUCCESSFULLY\n", targetAccount);
    printf("===============================================\n");

    logTransaction("DELETE_ACCOUNT", targetAccount, 0.0,
                   "FULL_VERIFICATION+LOOP_CONFIRM");

    return true;
}

#endif
