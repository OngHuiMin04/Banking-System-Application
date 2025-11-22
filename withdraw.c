#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <errno.h> 
#include <limits.h>

bool withdrawMoney() {
    printf("\n=== Withdraw Money ===\n");
    
    char accountNumber[10], pin[PIN_LENGTH];
    double amount;
    
    if (isLoggedIn) {
        strcpy(accountNumber, currentUser.accountNumber);
        printf("Withdrawing from account: %s\n", accountNumber);
        printf("Current balance: RM %.2lf\n", currentUser.balance);
        
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
    
    if (!isLoggedIn) {
        printf("Current balance: RM %.2lf\n", account.balance);
    }
    
    // Get withdrawal amount
    char amountStr[20];
    do {
        printf("Enter amount to withdraw: ");
        if (scanf("%19s", amountStr) != 1) return false;
        
        if (!isPositiveNumber(amountStr)) {
            printf("Invalid amount. Please enter a positive number.\n");
            continue;
        }
        
        amount = atof(amountStr);
        if (amount > account.balance) {
            printf("Insufficient funds. Current balance: RM %.2lf\n", account.balance);
            return false;
        }
        
        if (!isValidAmount(amount)) {
            printf("Amount must be between RM 0.01 and RM 50,000.\n");
        }
    } while (!isValidAmount(amount));
    
    // Update balance
    account.balance -= amount;
    if (saveAccount(&account)) {
        printf("Withdrawal successful! New balance: RM %.2lf\n", account.balance);
        logTransaction("WITHDRAWAL", accountNumber, amount, NULL);
        
        if (isLoggedIn && strcmp(currentUser.accountNumber, accountNumber) == 0) {
            currentUser.balance = account.balance;
        }
        return true;
    } else {
        printf("Withdrawal failed!\n");
        return false;
    }
}