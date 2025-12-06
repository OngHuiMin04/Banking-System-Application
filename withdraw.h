#ifndef WITHDRAW_H
#define WITHDRAW_H

#include "common.h"

bool withdrawMoney(void);

/* ==========================================================
   WITHDRAW MONEY FUNCTION
   ========================================================== */
bool withdrawMoney(void) {

    printf("\n===============================================\n");
    printf("                 WITHDRAW MONEY                \n");
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
    if (!readLine(pinInput, sizeof(pinInput)) ||
        strcmp(pinInput, acc.pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    /* ======================================================
       SHOW CURRENT BALANCE
       ====================================================== */
    printf("\nCurrent Balance: RM %.2f\n", acc.balance);

    /* ======================================================
       ENTER WITHDRAWAL AMOUNT
       ====================================================== */
    char amountStr[32];
    double amount = 0.0;

    while (1) {
        printf("\nEnter Withdrawal Amount: ");   // <-- CLEANED UP, NO RANGE SHOWN

        if (!readLine(amountStr, sizeof(amountStr)))
            return false;

        /* VALIDATE NUMBER FORMAT */
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
            printf("Invalid input. Digits only.\n");
            continue;
        }

        amount = atof(amountStr);

        /* ENSURE WITHDRAWAL LIMITS */
        if (amount < 0.01 || amount > 50000.00) {
            printf("Amount must be between RM 0.01 and RM 50000.00.\n");
            continue;
        }

        /* INSUFFICIENT BALANCE CHECK */
        if (amount > acc.balance) {
            printf("Insufficient balance. Your balance is RM %.2f.\n", acc.balance);
            continue;
        }

        break;  // VALID AMOUNT
    }

    /* ======================================================
       UPDATE BALANCE
       ====================================================== */
    acc.balance -= amount;
    saveAccount(&acc);

    /* ======================================================
       DISPLAY RESULT
       ====================================================== */
    printf("\nWithdrawal Successful.\n\n");
    printf("New Balance: RM %.2f\n", acc.balance);

    logTransaction("WITHDRAW", accountNumber, amount, "");

    return true;
}

#endif
