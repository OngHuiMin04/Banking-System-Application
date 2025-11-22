#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <errno.h> 
#include <limits.h>

bool remitMoney() {
    printf("\n=== Transfer / Remittance ===\n");
    
    if (!isLoggedIn) {
        printf("You need to login first to make a transfer.\n");
        if (!loginToAccount()) {
            printf("Transfer cancelled.\n");
            return false;
        }
    }
    
    char receiverAccount[10], pin[PIN_LENGTH];
    double amount, fee = 0.0;
    
    printf("Transferring from account: %s\n", currentUser.accountNumber);
    printf("Current balance: RM %.2lf\n", currentUser.balance);
    
    printf("Enter 4-digit PIN to confirm: ");
    if (scanf("%4s", pin) != 1) return false;
    
    if (strcmp(currentUser.pin, pin) != 0) {
        printf("Invalid PIN! Transaction cancelled.\n");
        return false;
    }
    
    printf("Enter recipient account number: ");
    if (scanf("%9s", receiverAccount) != 1) return false;
    
    if (strcmp(currentUser.accountNumber, receiverAccount) == 0) {
        printf("Cannot transfer to the same account!\n");
        return false;
    }
    
    BankAccount receiver;
    if (!loadAccount(receiverAccount, &receiver)) {
        printf("Recipient account not found!\n");
        return false;
    }
    
    // Get transfer amount
    char amountStr[20];
    do {
        printf("Enter amount to transfer: ");
        if (scanf("%19s", amountStr) != 1) return false;
        
        if (!isPositiveNumber(amountStr)) {
            printf("Invalid amount. Please enter a positive number.\n");
            continue;
        }
        
        amount = atof(amountStr);
        
        // Calculate fee
        if (currentUser.type == SAVINGS && receiver.type == CURRENT) {
            fee = amount * 0.02;
        } else if (currentUser.type == CURRENT && receiver.type == SAVINGS) {
            fee = amount * 0.03;
        }
        
        double totalDeduction = amount + fee;
        
        if (totalDeduction > currentUser.balance) {
            printf("Insufficient funds. Required: RM %.2lf (Amount: RM %.2lf + Fee: RM %.2lf)\n", 
                   totalDeduction, amount, fee);
            return false;
        }
        
        if (!isValidAmount(amount)) {
            printf("Amount must be between RM 0.01 and RM 50,000.\n");
        }
    } while (!isValidAmount(amount));
    
    // Load fresh copies
    BankAccount sender, receiverFresh;
    loadAccount(currentUser.accountNumber, &sender);
    loadAccount(receiverAccount, &receiverFresh);
    
    // Perform transfer
    sender.balance -= (amount + fee);
    receiverFresh.balance += amount;
    
    if (saveAccount(&sender) && saveAccount(&receiverFresh)) {
        printf("Transfer successful!\n");
        printf("Transfer: RM %.2lf\n", amount);
        if (fee > 0) printf("Fee: RM %.2lf\n", fee);
        printf("Your new balance: RM %.2lf\n", sender.balance);
        
        logTransaction("REMITTANCE", sender.accountNumber, amount, receiverAccount);
        if (fee > 0) logTransaction("REMITTANCE_FEE", sender.accountNumber, fee, NULL);
        
        currentUser.balance = sender.balance;
        return true;
    } else {
        printf("Transfer failed!\n");
        return false;
    }
}