#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>   // For _mkdir() on Windows
#endif

#include "menu.h"
#include "common.h"

/* =========================================================
   DATABASE DIRECTORY SETUP
   ========================================================= */
void ensureDatabaseDirectory(void) {
#ifdef _WIN32
    _mkdir(DATABASE_DIR);      // Windows version
#else
    mkdir(DATABASE_DIR, 0777); // Linux / macOS version
#endif
}

/* =========================================================
   DATABASE FILE SETUP
   Creates index.txt and transaction.log if they do not exist
   ========================================================= */
void ensureDatabaseFiles(void) {
    FILE *fp;

    // Create or open index.txt (account list)
    fp = fopen(INDEX_FILE, "a");
    if (fp) fclose(fp);

    // Create or open transaction.log (transaction history)
    fp = fopen(TRANSACTION_LOG, "a");
    if (fp) fclose(fp);
}

/* =========================================================
   MAIN PROGRAM ENTRY
   ========================================================= */
int main(void) {

    /* -----------------------------------------------
       1. Ensure database directory & files exist
       ----------------------------------------------- */
    ensureDatabaseDirectory();
    ensureDatabaseFiles();

    /* -----------------------------------------------
       2. Initialize session (login, welcome, etc.)
       ----------------------------------------------- */
    initializeSession();

    /* -----------------------------------------------
       3. Start the full menu-driven banking system
       ----------------------------------------------- */
    runBankingSystem();

    return 0;
}
