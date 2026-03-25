# Secure Personnel Record System

A command-line personnel record management system written in C, designed with a  focus on data security and low-level memory management. *(Currently under active development)*

---

##  Features

| Feature | Description |
|---|---|
| **AES-256-CBC Encryption** | Sensitive data (Name, Department) is symmetrically encrypted before hitting the disk. |
| **SHA-256 Password Hashing** | Passwords are NEVER stored in plaintext. They are one-way hashed, meaning they cannot be recovered, only verified. |
| **Secure Memory Handling** | Sensitive buffers (like plaintext passwords) are scrubbed from stack memory using `memset` immediately after use. |
| **Raw Binary File I/O** | Encrypted `struct` payloads are written directly to `records.dat` using fast, low-level `fread()` and `fwrite()` operations. |

---

##  Build & Run

### Prerequisites
You need a C compiler, CMake, and the OpenSSL development libraries installed on your system.
* **Ubuntu/Debian:** `sudo apt install build-essential cmake libssl-dev`
* **Windows:** Use MSYS2 (MinGW-w64) and install `mingw-w64-x86_64-openssl` and `cmake`.

### Compile

    mkdir build
    cd build
    cmake ..
    make

### Run

    ./Secure_Personnel_Record_System

---

## 💻 Usage

    ===================================
      Secure Personnel Record System
         [AES-256 + SHA-256]
    ===================================
      1. Add Record
      2. View Records
      3. Verify Password
      4. Exit
    ===================================
      Enter your choice:

### Adding a Record

    Enter ID       : 101
    Enter Name     : Arda Basal
    Enter Dept     : Engineering
    Enter Password : super_secret_password
    Record saved! (name/dept AES-encrypted, password SHA-256 hashed)

### Viewing Records & Password Verification

    ID     Name                 Department           Password Hash (SHA-256)
    --------------------------------------------------------------------------------------------
    101    Arda Basal           Engineering          88d4266fd4e6338d13b845fcf289579d209c8978...

    Enter Record ID: 101
    Enter Password : super_secret_password
    Password MATCHES for record 101.



---

## 📂 Project Structure

    Secure-Personnel-System/
    ├── CMakeLists.txt  # Build configuration and OpenSSL linking
    ├── main.c          # Core logic, menus, and file I/O
    ├── crypto.h        # Crypto interface and key definitions
    ├── crypto.c        # OpenSSL implementation (AES & SHA-256)
    ├── records.dat     # Binary database (generated at runtime)
    └── README.md       # This file

---

## 📄 License

This project is released under the [MIT License](https://opensource.org/licenses/MIT). Feel free to use, modify, and distribute.
