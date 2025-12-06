#ifndef MENU_H
#define MENU_H

#include "common.h"
#include "create_acc.h"
#include "deposit.h"
#include "withdraw.h"
#include "remittance.h"
#include "delete_acc.h"
#include <ctype.h>

static char session_time[50];

/* ======================================================
   HELPER: CONVERT STRING TO LOWERCASE
   ====================================================== */
static void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

/* ======================================================
   HELPER: MAP KEYWORDS TO MENU CHOICE NUMBER
   ====================================================== */
static int keywordToChoice(char *input) {
    toLowerCase(input);

    if (!strcmp(input, "1") || !strcmp(input, "create"))        return 1;
    if (!strcmp(input, "2") || !strcmp(input, "deposit"))       return 2;
    if (!strcmp(input, "3") || !strcmp(input, "withdraw"))      return 3;
    if (!strcmp(input, "4") || !strcmp(input, "remittance"))    return 4;
    if (!strcmp(input, "5") || !strcmp(input, "delete"))        return 5;
    if (!strcmp(input, "6") || !strcmp(input, "exit"))          return 6;

    return -1;
}

/* ======================================================
   INITIALIZE SESSION
   ====================================================== */
void initializeSession(void) {
    srand((unsigned int)time(NULL));

    getCurrentDateTime(session_time, sizeof(session_time));
    int accounts = countExistingAccounts();

    printf("\n===============================================\n");
    printf("        WELCOME TO THE BANKING SYSTEM\n");
    printf("===============================================\n");
    printf("Session Start Time : %s\n", session_time);
    printf("Accounts in System : %d\n", accounts);
    printf("===============================================\n\n");

    logTransaction("SESSION_START", "-", 0.0, "");
}

/* ======================================================
   DISPLAY SESSION INFO BAR
   ====================================================== */
void displaySessionInfo(void) {
    int accounts = countExistingAccounts();

    printf("\n-----------------------------------------------\n");
    printf("[ SESSION: %s | ACCOUNTS: %d ]\n", session_time, accounts);
    printf("-----------------------------------------------\n");
}

/* ======================================================
   DISPLAY MAIN MENU
   ====================================================== */
void displayMenu(void) {

    printf("\n==================== MAIN MENU ====================\n");
    printf(" 1. Create New Account        (or type 'create')\n");
    printf(" 2. Deposit Money             (or type 'deposit')\n");
    printf(" 3. Withdraw Money            (or type 'withdraw')\n");
    printf(" 4. Remittance / Transfer     (or type 'remittance')\n");
    printf(" 5. Delete Account            (or type 'delete')\n");
    printf(" 6. Exit System               (or type 'exit')\n");
    printf("---------------------------------------------------\n");
    printf("Enter your choice: ");
}

/* ======================================================
   RUN THE FULL BANKING SYSTEM LOOP
   ====================================================== */
void runBankingSystem(void) {
    char input[100];
    int choice;

    while (1) {

        /* SHOW SESSION HEADER */
        displaySessionInfo();

        /* SHOW MENU */
        displayMenu();

        /* READ USER INPUT */
        if (!readLine(input, sizeof(input))) {
            printf("Input error. Please try again.\n");
            continue;
        }

        choice = keywordToChoice(input);

        if (choice == -1) {
            printf("Invalid option. Please enter a valid menu option.\n");
            continue;
        }

        /* PROCESS MENU SELECTION */
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
                printf("\n=========================================\n");
                printf("      THANK YOU FOR USING OUR SYSTEM\n");
                printf("=========================================\n");
                logTransaction("SESSION_END", "-", 0.0, "");
                return;
        }
    }
}

#endif
