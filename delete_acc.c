#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <errno.h> 
#include <limits.h>

bool deleteBankAccount() {
    printf("\n=== Delete Bank Account ===\n");
    
    if (!isLoggedIn) {
        printf("You need to login first to delete an account.\n");
        if (!loginToAccount()) {
            printf("Account deletion cancelled.\n");
            return false;
        }
    }
    
    char targetAccount[10];
    strcpy(targetAccount, currentUser.accountNumber);
    
    // Additional verification
    char lastFourID[5];
    strncpy(lastFourID, currentUser.id + strlen(currentUser.id) - 4, 4);
    lastFourID[4] = '\0';
    
    char inputLastFour[5];
    printf("Enter last 4 characters of ID for verification: ");
    if (scanf("%4s", inputLastFour) != 1) return false;
    
    char pin[PIN_LENGTH];
    printf("Enter 4-digit PIN to confirm deletion: ");
    if (scanf("%4s", pin) != 1) return false;
    
    if (strcmp(currentUser.pin, pin) != 0 || strcmp(lastFourID, inputLastFour) != 0) {
        printf("Authentication failed! Account deletion cancelled.\n");
        return false;
    }
    
    // Confirm deletion
    char confirm;
    printf("Are you sure you want to delete account %s? This action cannot be undone! (y/n): ", targetAccount);
    clearInputBuffer();
    scanf("%c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        if (deleteAccountFile(targetAccount)) {
            printf("Account deleted successfully!\n");
            logTransaction("DELETE_ACCOUNT", targetAccount, 0.0, NULL);
            logoutAccount();
            return true;
        } else {
            printf("Failed to delete account.\n");
            return false;
        }
    } else {
        printf("Deletion cancelled.\n");
        return false;
    }
}