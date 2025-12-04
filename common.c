// common.c (toolbox (helpers shared by all components))
#include "common.h"

/* ------------------ Input Helpers ------------------ */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

bool readLine(char *buffer, size_t size) {
    if (!fgets(buffer, (int)size, stdin))
        return false;
    buffer[strcspn(buffer, "\n")] = '\0';
    return true;
}

void getCurrentDateTime(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", info);
}

/* ------------------ Logging ------------------ */
void logTransaction(const char *action,
                    const char *account,
                    double amount,
                    const char *extra)
{
    FILE *fp = fopen(TRANSACTION_LOG, "a");
    if (!fp) return;

    char stamp[50];
    getCurrentDateTime(stamp, sizeof(stamp));

    fprintf(fp, "[%s] %-15s | Acc=%s | Amount=%.2f",
            stamp,
            action ? action : "-",
            account ? account : "-",
            amount);

    if (extra && extra[0] != '\0')
        fprintf(fp, " | Extra=%s", extra);

    fprintf(fp, "\n");
    fclose(fp);
}

/* ------------------ Index File ------------------ */
int countExistingAccounts(void) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return 0;

    int cnt = 0;
    char acc[ACCOUNT_NUM_LEN];

    while (fscanf(fp, "%s", acc) == 1)
        cnt++;

    fclose(fp);
    return cnt;
}

int loadAllAccounts(char accounts[][ACCOUNT_NUM_LEN]) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return 0;

    int count = 0;
    while (count < 100 && fscanf(fp, "%s", accounts[count]) == 1)
        count++;

    fclose(fp);
    return count;
}

bool removeFromIndexFile(const char *target) {
    FILE *fp = fopen(INDEX_FILE, "r");
    if (!fp) return false;

    FILE *temp = fopen("database/index_tmp.txt", "w");
    if (!temp) {
        fclose(fp);
        return false;
    }

    char acc[ACCOUNT_NUM_LEN];
    bool found = false;

    while (fscanf(fp, "%s", acc) == 1) {
        if (strcmp(acc, target) == 0)
            found = true;
        else
            fprintf(temp, "%s\n", acc);
    }

    fclose(fp);
    fclose(temp);

    remove(INDEX_FILE);
    rename("database/index_tmp.txt", INDEX_FILE);

    return found;
}

/* ------------------ Validation ------------------ */
bool validateName(const char *name) {
    return (name && strlen(name) > 0);
}

bool validateID(const char *id) {
    return (id && strlen(id) > 0);
}

bool validateAccountType(const char *input, char *outType) {
    if (!input || !outType)
        return false;

    char lower[ACCTYPE_LEN];
    strncpy(lower, input, ACCTYPE_LEN - 1);
    lower[ACCTYPE_LEN - 1] = '\0';

    for (int i = 0; lower[i]; i++)
        lower[i] = (char)tolower(lower[i]);

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
    if (!pin || strlen(pin) != 4)
        return false;

    for (int i = 0; i < 4; i++)
        if (!isdigit(pin[i])) return false;

    return true;
}

bool isPositiveNumber(const char *str) {
    if (!str || !str[0])
        return false;

    int dot = 0;

    for (int i = 0; str[i]; i++) {
        if (str[i] == '.') {
            if (++dot > 1) return false;
        } else if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool isValidAmount(double amount) {
    return amount >= 0.01 && amount <= 50000.0;
}

/* ------------------ Account Files ------------------ */
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
    for (int attempt = 0; attempt < 1000; attempt++) {

        int digits = 7 + rand() % 3;

        int min = 1;
        for (int i = 1; i < digits; i++)
            min *= 10;

        int max = min * 10 - 1;

        int num = min + rand() % (max - min + 1);

        char buf[ACCOUNT_NUM_LEN];
        snprintf(buf, sizeof(buf), "%d", num);

        if (!accountExists(buf)) {

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
    if (!fp) return false;

    fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%.2f\n",
            acc->accountNumber,
            acc->name,
            acc->id,
            acc->accountType,
            acc->pin,
            acc->balance);

    fclose(fp);
    return true;
}

bool loadAccount(const char *accountNumber, BankAccount *acc) {
    if (!accountNumber || !acc) return false;

    char path[128];
    snprintf(path, sizeof(path), "%s/%s.txt", DATABASE_DIR, accountNumber);

    FILE *fp = fopen(path, "r");
    if (!fp) return false;

    fgets(acc->accountNumber, ACCOUNT_NUM_LEN, fp);
    acc->accountNumber[strcspn(acc->accountNumber, "\n")] = '\0';

    fgets(acc->name, NAME_LEN, fp);
    acc->name[strcspn(acc->name, "\n")] = '\0';

    fgets(acc->id, ID_LEN, fp);
    acc->id[strcspn(acc->id, "\n")] = '\0';

    fgets(acc->accountType, ACCTYPE_LEN, fp);
    acc->accountType[strcspn(acc->accountType, "\n")] = '\0';

    fgets(acc->pin, PIN_LENGTH, fp);
    acc->pin[strcspn(acc->pin, "\n")] = '\0';

    fscanf(fp, "%lf", &acc->balance);

    fclose(fp);
    return true;
}

/* ------------------ PIN Verification ------------------ */
bool authenticateAccount(const char *accountNumber, const char *pin) {
    BankAccount acc;

    if (!loadAccount(accountNumber, &acc)) {
        printf("Account not found.\n");
        return false;
    }

    if (strcmp(acc.pin, pin) != 0) {
        printf("Incorrect PIN.\n");
        return false;
    }

    return true;
}
