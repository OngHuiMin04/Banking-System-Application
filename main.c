#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>   // Required for _mkdir() on Windows
#endif

#include "menu.h"
#include "common.h"

/* ----------------------------------------------------
   Create database directory if not exists
---------------------------------------------------- */
void ensureDatabaseDirectory(void) {
#ifdef _WIN32
    _mkdir(DATABASE_DIR);
#else
    mkdir(DATABASE_DIR, 0777);
#endif
}

/* ----------------------------------------------------
   Create index.txt and transaction.log if not exists
---------------------------------------------------- */
void ensureDatabaseFiles(void) {
    FILE *fp;

    // index.txt
    fp = fopen(INDEX_FILE, "a");
    if (fp) fclose(fp);

    // transaction.log
    fp = fopen(TRANSACTION_LOG, "a");
    if (fp) fclose(fp);
}

/* ----------------------------------------------------
   MAIN FUNCTION
---------------------------------------------------- */
int main(void) {
    // 1. Ensure required storage directories & files exist
    ensureDatabaseDirectory();
    ensureDatabaseFiles();

    // 2. Start session
    initializeSession();

    // 3. Run full system loop
    runBankingSystem();

    return 0;
}
