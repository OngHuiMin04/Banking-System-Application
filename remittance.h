#ifndef REMITTANCE_H
#define REMITTANCE_H

#include "common.h"

bool remitMoney(void);

/* ==========================================================
   TRANSFER / REMITTANCE FUNCTION
   ========================================================== */
bool remitMoney(void) {

    printf("\n===============================================\n");
    printf("              TRANSFER / REMITTANCE            \n");
    printf("===============================================\n");

    /* ======================================================
       LOAD ACCOUNT LIST
       ====================================================== */
    char accounts[100][ACCOUNT_NUM_LEN];
    int count = loadAllAccounts(accounts);

    if (count == 0) {
        printf("No accounts available.\n");
        return false;
    }

    /* ======================================================
       SELECT SENDER ACCOUNT
       ====================================================== */
    printf("\nSelect SENDER Account:\n");
    int senderIndex = showAccountSelection(accounts, count);
    if (senderIndex == -1) return false;

    char senderAcc[ACCOUNT_NUM_LEN];
    strcpy(senderAcc, accounts[senderIndex]);

    BankAccount sender;
    if (!loadAccount(senderAcc, &sender)) {
        printf("Error: Could not load sender account.\n");
        return false;
    }

    /* ======================================================
       PIN VERIFICATION (SENDER)
       ====================================================== */
    char pinInput[50];
    printf("\nEnter Sender 4-Digit PIN: ");
    if (!readLine(pinInput, sizeof(pinInput)) ||
        strcmp(pinInput, sender.pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    printf("\nSender verified.\n");

    /* ======================================================
       SELECT RECEIVER ACCOUNT
       ====================================================== */
    printf("\nSelect RECEIVER Account:\n");
    int receiverIndex = showAccountSelection(accounts, count);
    if (receiverIndex == -1) return false;

    if (receiverIndex == senderIndex) {
        printf("Cannot transfer to the same account.\n");
        return false;
    }

    char receiverAcc[ACCOUNT_NUM_LEN];
    strcpy(receiverAcc, accounts[receiverIndex]);

    BankAccount receiver;
    if (!loadAccount(receiverAcc, &receiver)) {
        printf("Error: Could not load receiver account.\n");
        return false;
    }

    printf("\n-----------------------------------------------\n");
    printf("FROM : %s (%s)\n", sender.accountNumber, sender.name);
    printf("TO   : %s (%s)\n", receiver.accountNumber, receiver.name);
    printf("-----------------------------------------------\n");

    /* ======================================================
       ENTER TRANSFER AMOUNT
       ====================================================== */
    char amountStr[32];
    double amount = 0.0;
    double fee = 0.0;
    double total = 0.0;

    while (1) {

        printf("\nEnter Transfer Amount: ");

        if (!readLine(amountStr, sizeof(amountStr)))
            return false;

        /* VALIDATE NUMBER FORMAT */
        bool valid = true;
        int dots = 0;

        for (int i = 0; amountStr[i] != '\0'; i++) {
            if (amountStr[i] == '.') {
                dots++;
                if (dots > 1) { valid = false; break; }
            }
            else if (!isdigit((unsigned char)amountStr[i])) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            printf("Invalid amount. Digits only.\n");
            continue;
        }

        amount = atof(amountStr);

        /* ==================================================
           INSUFFICIENT AMOUNT CHECK
           (ANY amount > sender balance should fail)
           ================================================== */
        if (amount < 0.01 || amount > sender.balance) {
            printf("Insufficient amount.\n");
            printf("Your Balance: RM %.2f\n", sender.balance);
            continue;
        }

        /* Internal safety against extreme inputs */
        if (amount > 50000.00) {
            printf("Insufficient amount.\n");
            printf("Your Balance: RM %.2f\n", sender.balance);
            continue;
        }

        break;
    }

    /* ======================================================
       FEE CALCULATION RULES
       ====================================================== */
    if (strcmp(sender.accountType, "Savings") == 0 &&
        strcmp(receiver.accountType, "Current") == 0) {
        fee = amount * 0.02;
    }
    else if (strcmp(sender.accountType, "Current") == 0 &&
             strcmp(receiver.accountType, "Savings") == 0) {
        fee = amount * 0.03;
    }
    else {
        fee = 0.0;
    }

    total = amount + fee;

    /* ======================================================
       BALANCE CHECK AFTER FEE
       ====================================================== */
    if (total > sender.balance) {
        printf("\nInsufficient amount.\n");
        printf("Transfer Amount : RM %.2f\n", amount);
        printf("Fee             : RM %.2f\n", fee);
        printf("Total Required  : RM %.2f\n", total);
        printf("Your Balance    : RM %.2f\n", sender.balance);
        return false;
    }

    /* ======================================================
       APPLY TRANSFER
       ====================================================== */
    sender.balance -= total;
    receiver.balance += amount;

    saveAccount(&sender);
    saveAccount(&receiver);

    /* ======================================================
       DISPLAY TRANSFER RECEIPT
       ====================================================== */
    printf("\n===============================================\n");
    printf("                TRANSFER SUCCESSFUL            \n");
    printf("===============================================\n\n");

    printf("Transferred To : %s (%s)\n", receiver.accountNumber, receiver.name);
    printf("Amount Sent    : RM %.2f\n", amount);
    printf("Fee Charged    : RM %.2f\n", fee);
    printf("-----------------------------------------------\n");
    printf("Sender New Balance : RM %.2f\n\n", sender.balance);

    /* LOG TRANSACTION */
    logTransaction("REMIT", senderAcc, amount, receiverAcc);
    if (fee > 0) logTransaction("REMIT_FEE", senderAcc, fee, "");

    return true;
}

#endif
