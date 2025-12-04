#include "menu.h"
#include "create_acc.h"
#include "deposit.h"
#include "withdraw.h"
#include "remittance.h"
#include "delete_acc.h"

static char session_time[50];

static void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

static int keywordToChoice(char *input) {
    toLowerCase(input);

    if (!strcmp(input, "1") || !strcmp(input, "create"))      return 1;
    if (!strcmp(input, "2") || !strcmp(input, "deposit"))     return 2;
    if (!strcmp(input, "3") || !strcmp(input, "withdraw"))    return 3;
    if (!strcmp(input, "4") || !strcmp(input, "remittance"))  return 4;
    if (!strcmp(input, "5") || !strcmp(input, "delete"))      return 5;
    if (!strcmp(input, "6") || !strcmp(input, "exit"))        return 6;

    return -1;
}

void initializeSession(void) {
    srand((unsigned int)time(NULL));

    getCurrentDateTime(session_time, sizeof(session_time));
    int accounts = countExistingAccounts();

    printf("=== WELCOME TO BANKING SYSTEM ===\n");
    printf("Session started: %s\n", session_time);
    printf("Accounts in system: %d\n", accounts);
    printf("====================================\n\n");

    logTransaction("SESSION_START", "-", 0.0, "");
}

void displaySessionInfo(void) {
    int accounts = countExistingAccounts();
    printf("\n[Session: %s | Accounts: %d]\n", session_time, accounts);
}

void displayMenu(void) {
    printf("\n=== Main Menu ===\n");
    printf("1. Create New Account (or 'create')\n");
    printf("2. Deposit Money (or 'deposit')\n");
    printf("3. Withdraw Money (or 'withdraw')\n");
    printf("4. Remittance / Transfer (or 'remittance')\n");
    printf("5. Delete Account (or 'delete')\n");
    printf("6. Exit System (or 'exit')\n");
    printf("Choose option: ");
}

void runBankingSystem(void) {
    char input[100];
    int choice;

    while (1) {
        displaySessionInfo();
        displayMenu();

        if (!readLine(input, sizeof(input))) {
            printf("Input error.\n");
            continue;
        }

        choice = keywordToChoice(input);

        if (choice == -1) {
            printf("Invalid option. Please try again.\n");
            continue;
        }

        switch (choice) {
            case 1:
                createNewAccount();
                break;

            case 2:
                depositMoney();
                break;

            case 3:
                withdrawMoney();
                break;

            case 4:
                remitMoney();
                break;

            case 5:
                deleteBankAccount();
                break;

            case 6:
                printf("Thank you for using Banking System.\n");
                logTransaction("SESSION_END", "-", 0.0, "");
                return;
        }
    }
}
