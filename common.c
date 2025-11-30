// common.c (toolbox (helpers shared by all components))
#include "common.h"

bool isLoggedIn = false;
BankAccount currentUser;

// ---------- small helpers ----------
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

bool readLine(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return false;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    return true;
}

void getCurrentDateTime(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

// ---------- logging ----------
void logTransaction(const char *action,
                    const char *account,
                    double amount,
                    const char *extra) {
    FILE *fp = fopen(TRANSACTION_LOG, "a");
    if (!fp) return;

    char timestamp[50];
    getCurrentDateTime(timestamp, sizeof(timestamp));

    fprintf(fp, "[%s] %-15s | Acc=%s | Amount=%.2f",
            timestamp,
            action ? action : "-",
            account ? account : "-",
            amount);

    if (extra && extra[0] != '\0') {
        fprintf(fp, " | Extra=%s", extra);
    }
    fprintf(fp, "\n");

    fclose(fp);
}

// ---------- index & account count ----------
int countExistingAccounts(void) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return 0;

    int count = 0;
    char acc[ACCOUNT_NUM_LEN];
    while (fscanf(fp, "%s", acc) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

int loadAllAccounts(char accounts[][ACCOUNT_NUM_LEN]) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return 0;

    int count = 0;
    while (count < 100 && fscanf(fp, "%s", accounts[count]) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

bool removeFromIndexFile(const char *target) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return false;

    FILE *tmp = fopen("database/index_tmp.txt", "w");
    if (!tmp) {
        fclose(fp);
        return false;
    }

    char acc[ACCOUNT_NUM_LEN];
    bool found = false;

    while (fscanf(fp, "%s", acc) == 1) {
        if (strcmp(acc, target) == 0) {
            found = true;  // skip writing
        } else {
            fprintf(tmp, "%s\n", acc);
        }
    }

    fclose(fp);
    fclose(tmp);

    remove(INDEX_FILE);
    rename("database/index_tmp.txt", INDEX_FILE);

    return found;
}

// ---------- validation ----------
bool validateName(const char *name) {
    return name && strlen(name) > 0;
}

bool validateID(const char *id) {
    return id && strlen(id) > 0;
}

bool validateAccountType(const char *input, char *outType) {
    if (!input || !outType) return false;

    char lower[ACCTYPE_LEN];
    strncpy(lower, input, ACCTYPE_LEN - 1);
    lower[ACCTYPE_LEN - 1] = '\0';

    for (int i = 0; lower[i]; i++) {
        lower[i] = (char)tolower((unsigned char)lower[i]);
    }

    if (strcmp(lower, "savings") == 0) {
        strcpy(outType, "Savings");
        return true;
    }
    if (strcmp(lower, "current") == 0) {
        strcpy(outType, "Current");
        return true;
    }
    return false;
}

bool validatePIN(const char *pin) {
    if (!pin) return false;
    if (strlen(pin) != 4) return false;
    for (int i = 0; i < 4; i++) {
        if (!isdigit((unsigned char)pin[i])) return false;
    }
    return true;
}

bool isPositiveNumber(const char *str) {
    if (!str || !str[0]) return false;

    int dotCount = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1) return false;
        } else if (!isdigit((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}

bool isValidAmount(double amount) {
    return amount >= 0.01 && amount <= 50000.0;
}

// ---------- account files ----------
bool accountExists(const char *accountNumber) {
    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, accountNumber);

    FILE *fp = fopen(path, "r");
    if (fp) {
        fclose(fp);
        return true;
    }
    return false;
}

int generateAccountNumber(void) {
    // 7 to 9 digits
    for (int attempts = 0; attempts < 1000; attempts++) {
        int digits = 7 + rand() % 3;
        int min = 1;
        for (int i = 1; i < digits; i++) min *= 10;

        int max = min * 10 - 1;
        int num = min + rand() % (max - min + 1);

        char buf[ACCOUNT_NUM_LEN];
        snprintf(buf, sizeof(buf), "%d", num);

        if (!accountExists(buf)) {
            // append to index file
            FILE *idx = fopen(INDEX_FILE, "a");
            if (!idx) return -1;
            fprintf(idx, "%s\n", buf);
            fclose(idx);
            return num;
        }
    }
    return -1;
}

bool saveAccount(const BankAccount *acc) {
    if (!acc) return false;

    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, acc->accountNumber);

    FILE *fp = fopen(path, "w");
    if (!fp) {
        printf("Error: cannot open account file for writing.\n");
        return false;
    }

    fprintf(fp, "%s\n", acc->accountNumber);
    fprintf(fp, "%s\n", acc->name);
    fprintf(fp, "%s\n", acc->id);
    fprintf(fp, "%s\n", acc->accountType);
    fprintf(fp, "%s\n", acc->pin);
    fprintf(fp, "%.2f\n", acc->balance);

    fclose(fp);
    return true;
}

bool loadAccount(const char *accountNumber, BankAccount *acc) {
    if (!accountNumber || !acc) return false;

    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, accountNumber);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        return false;
    }

    if (!fgets(acc->accountNumber, ACCOUNT_NUM_LEN, fp)) { fclose(fp); return false; }
    acc->accountNumber[strcspn(acc->accountNumber, "\n")] = '\0';

    if (!fgets(acc->name, NAME_LEN, fp)) { fclose(fp); return false; }
    acc->name[strcspn(acc->name, "\n")] = '\0';

    if (!fgets(acc->id, ID_LEN, fp)) { fclose(fp); return false; }
    acc->id[strcspn(acc->id, "\n")] = '\0';

    if (!fgets(acc->accountType, ACCTYPE_LEN, fp)) { fclose(fp); return false; }
    acc->accountType[strcspn(acc->accountType, "\n")] = '\0';

    if (!fgets(acc->pin, PIN_LENGTH, fp)) { fclose(fp); return false; }
    acc->pin[strcspn(acc->pin, "\n")] = '\0';

    if (fscanf(fp, "%lf", &acc->balance) != 1) { fclose(fp); return false; }

    fclose(fp);
    return true;
}

bool authenticateAccount(const char *accountNumber, const char *pin) {
    BankAccount acc;
    if (!loadAccount(accountNumber, &acc)) {
        printf("Account not found.\n");
        return false;
    }
    if (strcmp(acc.pin, pin) != 0) {
        printf("Invalid PIN.\n");
        return false;
    }
    return true;
}
