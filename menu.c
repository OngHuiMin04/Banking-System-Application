#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#include <errno.h> 
#include <limits.h>

char session_time[50];
int accounts_loaded = 0;
bool logged_in = false;
char current_account[50];

// Convert string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}
 
// Keyword OR number → menu choice
int keywordToChoice(char *input) {
    toLowerCase(input);

    if (strcmp(input, "1") == 0 || strcmp(input, "create") == 0) return 1;
    if (strcmp(input, "2") == 0 || strcmp(input, "login") == 0) return 2;
    if (strcmp(input, "3") == 0 || strcmp(input, "deposit") == 0) return 3;
    if (strcmp(input, "4") == 0 || strcmp(input, "withdraw") == 0) return 4;
    if (strcmp(input, "5") == 0 || strcmp(input, "remittance") == 0) return 5;
    if (strcmp(input, "6") == 0 || strcmp(input, "delete") == 0) return 6;
    if (strcmp(input, "7") == 0 || strcmp(input, "logout") == 0) return 7;
    if (strcmp(input, "8") == 0 || strcmp(input, "exit") == 0) return 8;

    return -1;
}

// Get Date/Time (Required)
void getCurrentDateTime(char *buffer, int size) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Log Transaction
void logTransaction(const char *from, const char *to, double amount, const char *type) {
    FILE *file = fopen("database/transaction.log", "a");
    if (!file) return;

    char timestamp[50];
    getCurrentDateTime(timestamp, sizeof(timestamp));

    fprintf(file, "[%s] %s: From=%s, To=%s, Amount=%.2f\n",
            timestamp, type, from, to, amount);

    fclose(file);
}

// Initialize Session
void initializeSession() {
    getCurrentDateTime(session_time, sizeof(session_time));
    accounts_loaded = countExistingAccounts();

    printf("=== HM BANKING SYSTEM ===\n");
    printf("Session started: %s\n", session_time);
    printf("Accounts in system: %d\n", accounts_loaded);
    printf("==================================\n\n");

    logTransaction("SYSTEM", "SYSTEM", 0, "Session started");
}

// Display Session Info (shown before each menu)
void displaySessionInfo() {
    printf("Session: %s | Accounts: %d", session_time, accounts_loaded);
    if (logged_in) {
        printf(" | Logged in: %s", current_account);
    }
    printf("\n");
}

// Display Menu
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
    printf("Choose option: ");
}

// Run Banking System
void runBankingSystem() {
    char input[100];
    int choice;

    while (1) {
        displaySessionInfo();
        displayMenu();

        // Get user input as either number OR keyword
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        choice = keywordToChoice(input);

        if (choice == -1) {
            printf("Invalid option. Please try again.\n");
            continue;
        }

        switch (choice) {
            case 1:
                createNewAccount();
                accounts_loaded = countExistingAccounts();
                break;

            case 2:
                loginToAccount();
                break;

            case 3:
                if (!logged_in)
                    printf("Please login first to deposit money.\n");
                else
                    depositMoney();
                break;

            case 4:
                if (!logged_in)
                    printf("Please login first to withdraw money.\n");
                else
                    withdrawMoney();
                break;

            case 5:
                if (!logged_in)
                    printf("Please login first to transfer money.\n");
                else
                    performRemittance();
                break;

            case 6:
                if (!logged_in)
                    printf("Please login first to delete your account.\n");
                else {
                    deleteAccount();
                    accounts_loaded = countExistingAccounts();
                }
                break;

            case 7:
                logoutUser();
                break;

            case 8:
                exitSystem();
                return;
        }
    }
}
