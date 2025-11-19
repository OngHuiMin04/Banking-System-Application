#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <errno.h> 
#include <limits.h>

void loginToAccount() {
    char account_num[10];
    char pin[PIN_LENGTH];
    
    printf("\n=== Login to Account ===\n");
    printf("Enter account number: ");

    if (fgets(account_num, sizeof(account_num), stdin) == NULL) {
        printf("Error reading input.\n");
        return;
    }
    
     // Get account number
    while (1) {
        printf("Enter account number: ");
        if (fgets(account_num, sizeof(account_num), stdin) != NULL) {
            account_number[strcspn(account_num, "\n")] = 0;
            if (strlen(account_number) > 0) {
                break;
            }
        }
        printf("Invalid account number.\n");
    }
    
    // Get PIN
    while (1) {
        printf("Enter 4-digit PIN: ");
        if (fgets(pin, sizeof(pin), stdin) != NULL) {
            pin[strcspn(pin, "\n")] = 0;
            if (is_valid_pin(pin)) {
                break;
            }
        }
        printf("Invalid PIN format.\n");
    }
    
  
    if (authenticateAccount(account_num, pin)) {
        logged_in = true;
        strcpy(current_account, account_num);
        printf("Login successful! Welcome account %s\n", current_account);
        printf(" Welcome %s!\n", account->name);
        printf(" Account Summary:\n");
        printf("   Account Number: %s\n", account->account_number);
        printf("   Account Type: %s\n", account->type);
        printf("   Current Balance: RM%.2f\n", account->balance);
        logTransaction("LOGIN", account_num, 0, "");
    } else {
        printf("Login failed. Please check your account number and PIN.\n");
    }
}