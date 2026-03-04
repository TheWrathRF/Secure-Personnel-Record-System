#  Secure Personnel Record System

A command-line personnel record management system,featuring binary file storage and XOR-based password encryption.



---

##  Features

| Feature              | Description                                                    |
|----------------------|----------------------------------------------------------------|
| **Struct-based data** | Personnel records use a clean `struct` with ID, Name, Dept, and Password fields. |
| **Binary File I/O**  | Records are persisted to `records.dat` using `fwrite()` / `fread()` for efficient storage. |
| **XOR Encryption**   | Passwords are encrypted with a symmetric XOR cipher before being written to disk. |
| **Error Handling**    | Graceful handling of invalid menu input and file-open failures. |

---

##  Build & Run

### Prerequisites

- C compiler (`gcc`, `clang`, or MSVC)

### Compile

```bash
gcc -Wall -Wextra -o personnel main.c
```

### Run

```bash
./personnel
```

On Windows (PowerShell):
```powershell
.\personnel.exe
```

---

##  Usage

```
===================================
  Secure Personnel Record System
===================================
  1. Add Record
  2. View Records
  3. Exit
===================================
  Enter your choice:
```

### Adding a Record

```
--- Add New Record ---
Enter ID       : 101
Enter Name     : Arda Başal
Enter Dept     : Engineering
Enter Password : s3cureP@ss
Record saved successfully! (password encrypted)
```

### Viewing Records

```
ID     Name                 Department           Password
--------------------------------------------------------------
101    Arda Başal          Engineering          s3cureP@ss
```

> Passwords are stored **encrypted** in the binary file but displayed in **plain text** after decryption.

---

##  How XOR Encryption Works

Each character of the password is XORed with a secret key (`'K'`):

```
Encrypt:   'H' ^ 'K' = 0x03  (garbled byte)
Decrypt:   0x03 ^ 'K' = 'H'  (original restored)
```

Because XOR is its own inverse, the same function handles both encryption and decryption. 

---

##  Project Structure

```
Secure-Personnel-System/
├── main.c          # All source code (struct, menu, CRUD, encryption)
├── records.dat     # Binary data file (created at runtime)
└── README.md       # This file
```






## 📄 License

This project is released under the [MIT License](https://opensource.org/licenses/MIT). Feel free to use, modify, and distribute.
