#include "remittance.h"

bool remitMoney(void) {
    printf("\n=== Transfer / Remittance ===\n");

    char senderAccNum[ACCOUNT_NUM_LEN];
    char receiverAccNum[ACCOUNT_NUM_LEN];
    char pin[PIN_LENGTH];
    char amountStr[32];
    double amount, fee = 0.0;

    // Sender account number
    printf("Enter sender account number: ");
    if (!readLine(senderAccNum, sizeof(senderAccNum))) return false;

    // PIN
    printf("Enter 4-digit PIN: ");
    if (!readLine(pin, sizeof(pin)) || !validatePIN(pin)) {
        printf("Invalid PIN.\n");
        return false;
    }

    // Load sender account
    BankAccount sender;
    if (!loadAccount(senderAccNum, &sender)) {
        printf("Sender account not found.\n");
        return false;
    }

    if (strcmp(sender.pin, pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    printf("Sender balance: RM %.2f\n", sender.balance);

    // Receiver account
    printf("Enter receiver account number: ");
    if (!readLine(receiverAccNum, sizeof(receiverAccNum))) return false;

    if (strcmp(senderAccNum, receiverAccNum) == 0) {
        printf("Cannot transfer to the same account.\n");
        return false;
    }

    BankAccount receiver;
    if (!loadAccount(receiverAccNum, &receiver)) {
        printf("Receiver account not found.\n");
        return false;
    }

    // Enter amount
    while (1) {
        printf("Enter amount to transfer (RM 0.01 - RM 50,000.00): ");
        if (!readLine(amountStr, sizeof(amountStr))) return false;

        if (!isPositiveNumber(amountStr)) {
            printf("Please enter numbers only.\n");
            continue;
        }

        amount = atof(amountStr);
        if (!isValidAmount(amount)) {
            printf("Amount must be between RM 0.01 - RM 50,000.\n");
            continue;
        }

        // Fees based on account types
        if (strcmp(sender.accountType, "Savings") == 0 &&
            strcmp(receiver.accountType, "Current") == 0) {
            fee = amount * 0.02;
        }
        else if (strcmp(sender.accountType, "Current") == 0 &&
                 strcmp(receiver.accountType, "Savings") == 0) {
            fee = amount * 0.03;
        }
        else {
            fee = 0;
        }

        double total = amount + fee;
        if (total > sender.balance) {
            printf("Insufficient funds (need RM %.2f including fee RM %.2f).\n",
                   total, fee);
            return false;
        }

        break;
    }

    // Reload accounts before updating
    BankAccount freshSender, freshReceiver;
    if (!loadAccount(senderAccNum, &freshSender) ||
        !loadAccount(receiverAccNum, &freshReceiver)) {
        printf("Error loading account files.\n");
        return false;
    }

    // Update balances
    freshSender.balance -= (amount + fee);
    freshReceiver.balance += amount;

    // Save changes
    if (!saveAccount(&freshSender) || !saveAccount(&freshReceiver)) {
        printf("Failed to save updated accounts.\n");
        return false;
    }

    printf("\nTransfer successful!\n");
    printf("Transferred: RM %.2f\n", amount);
    if (fee > 0) printf("Fee charged: RM %.2f\n", fee);
    printf("New sender balance: RM %.2f\n", freshSender.balance);

    // Logging
    logTransaction("REMITTANCE", senderAccNum, amount, receiverAccNum);
    if (fee > 0) {
        logTransaction("REMITTANCE_FEE", senderAccNum, fee, "");
    }

    return true;
}