# Banking System Application  
*A Console-Based Banking Simulation System Written in C*

Developed as part of **COMP1312 Programming I Coursework**, this project demonstrates core C programming concepts including file handling, input validation, structured programming, data persistence, and modular design.

---

## 📌 1. Overview

The Banking System Application is a **console-based banking program** written entirely in **C**, designed to simulate real-world banking operations:

- Create New Account  
- Deposit Money  
- Withdraw Money  
- Transfer / Remittance  
- Delete Account  
- View Session Information  

All accounts and logs are stored in the automatically-generated **database** directory:

database/
│── index.txt
│── transaction.log
└── <accountNumber>.txt


Each transaction is recorded with a timestamp for accurate tracking.

---

## 📌 2. Features

### 🔹 Create New Account
- Validates name, ID, account type, and PIN  
- Generates unique 7–9 digit account number  
- Creates account file under `database/`  
- Initial balance = RM 0.00  
- **Log generated:** `CREATE_ACCOUNT`

---

### 🔹 Deposit Money
- Select account  
- Authenticate using PIN  
- Enter valid deposit amount (RM 0.01 – RM 50,000)  
- Updates account balance  
- **Log generated:** `DEPOSIT`

---

### 🔹 Withdraw Money
- Select account  
- Authenticate PIN  
- System displays current balance  
- Enter withdrawal amount (must not exceed balance)  
- **Log generated:** `WITHDRAW`  

---

### 🔹 Transfer / Remittance
Fee rules:

| Sender → Receiver | Fee |
|------------------|------|
| Savings → Current | 2%  |
| Current → Savings | 3%  |
| Same Type        | No Fee |

Sender pays:  
`Total = Amount + Fee`  

Receiver receives:  
`Amount`

Logs generated:  
- `REMIT`  
- `REMIT_FEE` (when applicable)

---

### 🔹 Delete Account
Three-step verification:

1. Re-enter account number  
2. Enter last 4 digits of ID  
3. Enter PIN  
4. Confirm by typing `yes`  

Deletes:  
- The account file  
- The account number in `index.txt`  

Log generated: `DELETE_ACCOUNT`

---

### 🔹 Session Information Display
Displayed before every menu:

[ SESSION: <start-date/time> | ACCOUNTS: <count> ]


Shows number of active accounts and session timestamp.

---

## 📌 3. How to Compile & Run

### **Compile**
```bash
gcc main.c -o banking  

Run
.\banking

The program auto-creates:
database/
├── index.txt
└── transaction.log

## 📌 4. File Structure

### 📁 database/index.txt
Stores all existing account numbers.  
Used to prevent duplicate account creation.

---

### 📁 database/transaction.log
Contains recorded actions:

- SESSION_START / SESSION_END  
- CREATE_ACCOUNT  
- DEPOSIT  
- WITHDRAW  
- REMIT / REMIT_FEE  
- DELETE_ACCOUNT  

Each log line includes a timestamp.

---

### 📁 database/<accountNumber>.txt
Contains:
Account Number
Name
ID
Account Type
PIN
Balance

Updated after every transaction.

## 📌 5. Project Structure
/project
│── common.h
│── menu.h
│── create_acc.h
│── deposit.h
│── withdraw.h
│── remittance.h
│── delete_acc.h
│── main.c
└── database/


All modules follow structured, testable, and maintainable programming practices.

---

## 📌 6. Documentation
A detailed instruction manual (PDF) is included in the repository.

---

## 📌 7. Author
**Ong Hui Min**  
University of Southampton Malaysia  
BSc Computer Science (Year 1)

GitHub: https://github.com/OngHuiMin04

---

## 📌 8. License
This project is developed for **COMP1312 Coursework**.  
Please do not reuse or submit without permission.