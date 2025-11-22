#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <errno.h> 
#include <limits.h>


bool depositMoney() {
    printf("\n=== Deposit Money ===\n");
    
    char accountNumber[10], pin[PIN_LENGTH];
    double amount;
    
    if (isLoggedIn) {
        strcpy(accountNumber, currentUser.accountNumber);
        printf("Depositing to account: %s\n", accountNumber);
        
        printf("Enter 4-digit PIN to confirm: ");
        if (scanf("%4s", pin) != 1) return false;
        
        if (strcmp(currentUser.pin, pin) != 0) {
            printf("Invalid PIN! Transaction cancelled.\n");
            return false;
        }
    } else {
        printf("Enter account number: ");
        if (scanf("%9s", accountNumber) != 1) return false;
        
        printf("Enter 4-digit PIN: ");
        if (scanf("%4s", pin) != 1) return false;
        
        if (!authenticateAccount(accountNumber, pin)) return false;
    }
    
    BankAccount account;
    loadAccount(accountNumber, &account);
    
    // Get deposit amount
    char amountStr[20];
    do {
        printf("Enter amount to deposit (RM 0.01 - 50,000): ");
        if (scanf("%19s", amountStr) != 1) return false;
        
        if (!isPositiveNumber(amountStr)) {
            printf("Invalid amount. Please enter a positive number.\n");
            continue;
        }
        
        amount = atof(amountStr);
        if (!isValidAmount(amount)) {
            printf("Amount must be between RM 0.01 and RM 50,000.\n");
        }
    } while (!isValidAmount(amount));
    
    // Update balance
    account.balance += amount;
    if (saveAccount(&account)) {
        printf("Deposit successful! New balance: RM %.2lf\n", account.balance);
        logTransaction("DEPOSIT", accountNumber, amount, NULL);
        
        if (isLoggedIn && strcmp(currentUser.accountNumber, accountNumber) == 0) {
            currentUser.balance = account.balance;
        }
        return true;
    } else {
        printf("Deposit failed!\n");
        return false;
    }
}