

void logoutAccount() {
    if (!isLoggedIn) {
        printf("No user is currently logged in.\n");
        return;
    }
    
    printf("Logging out account: %s\n", currentUser.accountNumber);
    logTransaction("LOGOUT", currentUser.accountNumber, 0.0, NULL);
    
    isLoggedIn = false;
    memset(&currentUser, 0, sizeof(BankAccount));
    printf("Logout successful!\n");
}