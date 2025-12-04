#include "create_acc.h"
#include "deposit.h"
#include "withdraw.h"
#include "remittance.h"
#include "delete_acc.h"
#include "common.h"
#include "menu.h"

int main() {
    srand((unsigned int)time(NULL));

    int choice;
    char buf[10];

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

    while (1) {
        printf("\n=== WELCOME TO BANKING SYSTEM ===\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer / Remittance\n");
        printf("5. Delete Bank Account\n");
        printf("6. Exit\n");
        printf("Choose an option: ");

        if (!readLine(buf, sizeof(buf))) continue;
        choice = atoi(buf);

        switch (choice) {
            case 1: createNewAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: remitMoney(); break;
            case 5: deleteBankAccount(); break;

            case 6:
                printf("Thank you for using Banking System.\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

