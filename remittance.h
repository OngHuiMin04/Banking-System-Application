#ifndef REMITTANCE_H
#define REMITTANCE_H

#include "common.h"

bool remitMoney(void);

bool remitMoney(void) {
    printf("\n========== TRANSFER / REMITTANCE ==========\n");

    /* ----------------------------------------
       LOAD ACCOUNT LIST
    ---------------------------------------- */
    char accounts[100][ACCOUNT_NUM_LEN];
    int count = loadAllAccounts(accounts);

    if (count == 0) {
        printf("No accounts available.\n");
        return false;
    }

    /* ----------------------------------------
       SELECT SENDER ACCOUNT
    ---------------------------------------- */
    printf("\nChoose SENDER account:\n");
    int senderIndex = showAccountSelection(accounts, count);
    if (senderIndex == -1) return false;

    char senderAcc[ACCOUNT_NUM_LEN];
    strcpy(senderAcc, accounts[senderIndex]);

    BankAccount sender;
    if (!loadAccount(senderAcc, &sender)) {
        printf("Error loading sender account.\n");
        return false;
    }

    /* ----------------------------------------
       PIN VERIFICATION (SENDER)
    ---------------------------------------- */
    char pinInput[50];
    printf("Enter sender 4-digit PIN: ");
    if (!readLine(pinInput, sizeof(pinInput)) || strcmp(pinInput, sender.pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    printf("\nSender verified successfully.\n");

    /* ----------------------------------------
       SELECT RECEIVER ACCOUNT
    ---------------------------------------- */
    printf("\nChoose RECEIVER account:\n");
    int receiverIndex = showAccountSelection(accounts, count);
    if (receiverIndex == -1) return false;

    if (receiverIndex == senderIndex) {
        printf("Cannot transfer to the SAME account.\n");
        return false;
    }

    char receiverAcc[ACCOUNT_NUM_LEN];
    strcpy(receiverAcc, accounts[receiverIndex]);

    BankAccount receiver;
    if (!loadAccount(receiverAcc, &receiver)) {
        printf("Error loading receiver account.\n");
        return false;
    }

    printf("\nFrom: %s (%s)\n", sender.accountNumber, sender.name);
    printf("To  : %s (%s)\n", receiver.accountNumber, receiver.name);

    /* ----------------------------------------
       ENTER TRANSFER AMOUNT
    ---------------------------------------- */
    char amountStr[32];
    double amount = 0.0, fee = 0.0, total = 0.0;

    while (1) {
        printf("\nEnter transfer amount (RM 0.01 - RM 50000.00): ");
        if (!readLine(amountStr, sizeof(amountStr)))
            return false;

        // Validate number formatting
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
            printf("Invalid amount. Please enter numbers only.\n");
            continue;
        }

        amount = atof(amountStr);

        if (amount < 0.01 || amount > 50000.00) {
            printf("Amount must be between RM 0.01 and RM 50000.00.\n");
            continue;
        }

        break;
    }

    /* ----------------------------------------
       FEE CALCULATION RULES
    ---------------------------------------- */
    fee = 0.0;

    if (strcmp(sender.accountType, "Savings") == 0 &&
        strcmp(receiver.accountType, "Current") == 0)
        fee = amount * 0.02;

    else if (strcmp(sender.accountType, "Current") == 0 &&
             strcmp(receiver.accountType, "Savings") == 0)
        fee = amount * 0.03;

    total = amount + fee;

    /* ----------------------------------------
       BALANCE CHECK
    ---------------------------------------- */
    if (total > sender.balance) {
        printf("\nInsufficient funds.\n");
        printf("Transfer Amount : RM %.2f\n", amount);
        printf("Fee             : RM %.2f\n", fee);
        printf("Total Required  : RM %.2f\n", total);
        printf("Your Balance    : RM %.2f\n", sender.balance);
        return false;
    }

    /* ----------------------------------------
       APPLY TRANSFER
    ---------------------------------------- */
    sender.balance -= total;
    receiver.balance += amount;

    saveAccount(&sender);
    saveAccount(&receiver);

    /* ----------------------------------------
       DISPLAY RECEIPT
    ---------------------------------------- */
    printf("\n========== TRANSFER SUCCESSFUL ==========\n\n");
    printf("Transferred To : %s (%s)\n", receiver.accountNumber, receiver.name);
    printf("Amount Sent    : RM %.2f\n", amount);
    printf("Fee Charged    : RM %.2f\n", fee);
    printf("-----------------------------------------\n");
    printf("Sender New Balance : RM %.2f\n\n", sender.balance);

    /* Log transactions */
    logTransaction("REMIT", senderAcc, amount, receiverAcc);
    if (fee > 0) logTransaction("REMIT_FEE", senderAcc, fee, "");

    return true;
}

#endif
