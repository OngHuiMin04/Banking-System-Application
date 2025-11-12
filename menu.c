#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <errno.h> 
#include <limits.h>

void initializeSession() {
    getCurrentDateTime(session_time, sizeof(session_time));
    accounts_loaded = countExistingAccounts();
    
    printf("=== HM BANKING SYSTEM ===\n");
    printf("Session started: %s\n", session_time);
    printf("Accounts in system: %d\n", accounts_loaded);
    printf("==================================\n\n");
    
    logTransaction("SYSTEM", "SYSTEM", 0, "Session started");
}

void displaySessionInfo() {
    printf("Session: %s | Accounts: %d", session_time, accounts_loaded);
    if (logged_in) {
        printf(" | Logged in: %s", current_account);
    }
    printf("\n");
}

void displayMenu() {
    printf("\n=== HM BANKING SYSTEM ===\n");
    printf("------------------------------------------------------------\n\n");
    printf("            -- Main Menu --\n\n");
    printf("1. Create New Account (or type 'create')\n");
    printf("2. Login to Account (or type 'login')\n");
    printf("3. Deposit Money (or type 'deposit')\n");
    printf("4. Withdraw Money (or type 'withdraw')\n");
    printf("5. Remittance (or type 'remittance')\n");
    printf("6. Delete Account (or type 'delete')\n");
    printf("7. Logout (or type 'logout')\n");
    printf("8. Exit System (or type 'exit')\n");
    printf("\n------------------------------------------------------------\n");
    printf("Choose option (1-8): ");
}

void runBankingSystem() {
    int choice;
    
    while (1) {
        displaySessionInfo();
        displayMenu();
        choice = choose_input_checking(1, 8);
        
        switch (choice) {
            case 1:
                createNewAccount();
                accounts_loaded = countExistingAccounts();
                break;
            case 2:
                loginToAccount();
                break;
            case 3:
                if (!logged_in) {
                    printf("Please login first to deposit money.\n");
                } else {
                    depositMoney();
                }
                break;
            case 4:
                if (!logged_in) {
                    printf("Please login first to withdraw money.\n");
                } else {
                    withdrawMoney();
                }
                break;
            case 5:
                if (!logged_in) {
                    printf("Please login first to transfer money.\n");
                } else {
                    performRemittance();
                }
                break;
            case 6:
                if (!logged_in) {
                    printf("Please login first to delete your account.\n");
                } else {
                    deleteAccount();
                    accounts_loaded = countExistingAccounts();
                }
                break;
            case 7:
                logoutUser();
                break;
            case 8:
                exitSystem();
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    }
}

    void getCurrentDateTime(char *buffer, int size) {
    time_t rawtime;
    struct tm *timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
    }

    void logTransaction(const char *from, const char *to, double amount, const char *type) {
    FILE *file = fopen("database/transaction.log", "a");
    if (file != NULL) {
        char timestamp[50];
        getCurrentDateTime(timestamp, sizeof(timestamp));
        
        fprintf(file, "[%s] %s: From=%s, To=%s, Amount=%.2f\n", 
                timestamp, type, from, to, amount);
        fclose(file);
    }
}