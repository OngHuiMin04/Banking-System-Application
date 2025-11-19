#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <errno.h> 
#include <limits.h>

void createNewAccount() {
    BankAccount new_account;
    
    printf("\n=== Create New Account ===\n");

      // Get account holder name
    printf("Enter account holder name: ");
    if (fgets(new_account.name, sizeof(new_account.name), stdin) == NULL) {
        printf("Error reading input.\n");
        return;
    }
    new_account.name[strcspn(new_account.name, "\n")] = 0;
    
    if (!validateName(new_account.name)) {
        printf("Invalid name. Name cannot be empty.\n");
        return;
    }
    
    // Get identification number
    printf("Enter identification number (ID): ");
    if (fgets(new_account.id, sizeof(new_account.id), stdin) == NULL) {
        printf("Error reading input.\n");
        return;
    }
    new_account.id[strcspn(new_account.id, "\n")] = 0;
    
    if (!validateID(new_account.id)) {
        printf("Invalid ID. ID cannot be empty.\n");
        return;
    }
    
    // Get account type
    printf("Enter account type (Savings/Current): ");
    char type_input[20];
    if (fgets(type_input, sizeof(type_input), stdin) == NULL) {
        printf("Error reading input.\n");
        return;
    }
    type_input[strcspn(type_input, "\n")] = 0;
    
    if (!validateAccountType(type_input, new_account.account_type)) {
        printf("Invalid account type. Please enter 'Savings' or 'Current'.\n");
        return;
    }
    
    // Get 4-digit PIN
    printf("Enter 4-digit PIN: ");
    if (fgets(new_account.pin, sizeof(new_account.pin), stdin) == NULL) {
        printf("Error reading input.\n");
        return;
    }
    new_account.pin[strcspn(new_account.pin, "\n")] = 0;
    
    if (!validatePIN(new_account.pin)) {
        printf("Invalid PIN. PIN must be exactly 4 digits.\n");
        return;
    }
    
    // Generate unique account number
    int account_num = generateAccountNumber();
    if (account_num == -1) {
        printf("Error generating account number. Please try again.\n");
        return;
    }
    sprintf(new_account.account_number, "%d", account_num);
    
    // Set initial balance
    new_account.balance = 0.00;
    
    // Save account to file
    if (!saveAccount(&new_account)) {
        printf("Error saving account. Please try again.\n");
        return;
    }
    
    printf("\nAccount created successfully!\n");
    printf("Account Number: %s\n", new_account.account_number);
    printf("Account Type: %s\n", new_account.account_type);
    printf("Initial Balance: RM%.2f\n", new_account.balance);
    
    // Log transaction  
    logTransaction("CREATE_ACCOUNT", new_account.account_number, 0, "");
}
