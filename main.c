#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto.h"

#define DATA_FILE "records.dat"

/* 
 * AES-256-CBC with padding can expand up to block_size worth of bytes.
 * 50 bytes of plaintext -> at most 64 bytes of ciphertext (next 16-byte boundary).
 * 80 gives us some breathing room.
 */
#define ENC_FIELD_LEN 80

struct Personnel {
    int id;
    unsigned char enc_name[ENC_FIELD_LEN];   /* AES encrypted name */
    int enc_name_len;                         /* actual ciphertext length */
    unsigned char enc_dept[ENC_FIELD_LEN];   /* AES encrypted department */
    int enc_dept_len;
    char password_hash[65];                   /* SHA-256 hex digest (64 chars + \0) */
};


void addRecord(void);
void viewRecords(void);
void verifyPassword(void);
void drainStdin(void);


int main(void) {
    int choice = 0;
    char buffer[64];

    do {
        printf("\n===================================\n");
        printf("  Secure Personnel Record System\n");
        printf("     [AES-256 + SHA-256]\n");
        printf("===================================\n");
        printf("  1. Add Record\n");
        printf("  2. View Records\n");
        printf("  3. Verify Password\n");
        printf("  4. Exit\n");
        printf("===================================\n");
        printf("  Enter your choice: ");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\n\nEnd of input. Exiting.\n");
            break;
        }

        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("\nInvalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                addRecord();
                break;
            case 2:
                viewRecords();
                break;
            case 3:
                verifyPassword();
                break;
            case 4:
                printf("\nExiting. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Try again.\n");
                break;
        }

    } while (choice != 4);

    return 0;
}



void drainStdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}


void addRecord(void) {
    struct Personnel p;
    FILE *fp;
    char line[64];
    char raw_name[50];
    char raw_dept[50];
    char raw_pass[50];
    int enc_len;

    memset(&p, 0, sizeof(p));

    printf("\n--- Add New Record ---\n");

    /* ID */
    printf("Enter ID       : ");
    fflush(stdout);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Input ended. Record not saved.\n");
        return;
    }
    if (sscanf(line, "%d", &p.id) != 1 || p.id <= 0) {
        printf("Invalid ID (must be positive). Record not saved.\n");
        return;
    }

    /* Name */
    printf("Enter Name     : ");
    fflush(stdout);
    if (fgets(raw_name, sizeof(raw_name), stdin) == NULL) {
        printf("Input ended. Record not saved.\n");
        return;
    }
    if (strchr(raw_name, '\n') == NULL)
        drainStdin();
    raw_name[strcspn(raw_name, "\n")] = '\0';

    if (strlen(raw_name) == 0) {
        printf("Name cannot be empty. Record not saved.\n");
        return;
    }

    /* Department */
    printf("Enter Dept     : ");
    fflush(stdout);
    if (fgets(raw_dept, sizeof(raw_dept), stdin) == NULL) {
        printf("Input ended. Record not saved.\n");
        return;
    }
    if (strchr(raw_dept, '\n') == NULL)
        drainStdin();
    raw_dept[strcspn(raw_dept, "\n")] = '\0';

    if (strlen(raw_dept) == 0) {
        printf("Department cannot be empty. Record not saved.\n");
        return;
    }

    /* Password */
    printf("Enter Password : ");
    fflush(stdout);
    if (fgets(raw_pass, sizeof(raw_pass), stdin) == NULL) {
        printf("Input ended. Record not saved.\n");
        return;
    }
    if (strchr(raw_pass, '\n') == NULL)
        drainStdin();
    raw_pass[strcspn(raw_pass, "\n")] = '\0';

    if (strlen(raw_pass) == 0) {
        printf("Password cannot be empty. Record not saved.\n");
        return;
    }

    /* --- CRYPTO STARTS HERE --- */

    /* Encrypt name with AES-256-CBC */
    enc_len = aes_encrypt((unsigned char *)raw_name, (int)strlen(raw_name),
                          p.enc_name);
    if (enc_len < 0) {
        printf("Encryption failed for name. Record not saved.\n");
        return;
    }
    p.enc_name_len = enc_len;

    /* Encrypt dept with AES-256-CBC */
    enc_len = aes_encrypt((unsigned char *)raw_dept, (int)strlen(raw_dept),
                          p.enc_dept);
    if (enc_len < 0) {
        printf("Encryption failed for dept. Record not saved.\n");
        return;
    }
    p.enc_dept_len = enc_len;

    /* Hash password with SHA-256 — (never store the raw password) */
    sha256_hash(raw_pass, p.password_hash);
    memset(raw_pass, 0, sizeof(raw_pass));

    fp = fopen(DATA_FILE, "ab");
    if (fp == NULL) {
        perror("Error: Could not open file for writing");
        return;
    }

    if (fwrite(&p, sizeof(struct Personnel), 1, fp) != 1) {
        perror("Error: Could not write record to file");
        fclose(fp);
        return;
    }
    fclose(fp);

    printf("Record saved! (name/dept AES-encrypted, password SHA-256 hashed)\n");
}


void viewRecords(void) {
    struct Personnel p;
    FILE *fp;
    int count = 0;
    char dec_name[50];
    char dec_dept[50];
    int dec_len;

    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records file found. Add a record first.\n");
        return;
    }

    printf("\n%-6s %-20s %-20s %-66s\n",
           "ID", "Name", "Department", "Password Hash (SHA-256)");
    printf("----------------------------------------------"
           "----------------------------------------------\n");

    while (fread(&p, sizeof(struct Personnel), 1, fp) == 1) {

        /* Decrypt name */
        memset(dec_name, 0, sizeof(dec_name));
        dec_len = aes_decrypt(p.enc_name, p.enc_name_len,
                              (unsigned char *)dec_name);
        if (dec_len < 0) {
            strcpy(dec_name, "[DECRYPT ERROR]");
        } else {
            dec_name[dec_len] = '\0';
        }

        /* Decrypt dept */
        memset(dec_dept, 0, sizeof(dec_dept));
        dec_len = aes_decrypt(p.enc_dept, p.enc_dept_len,
                              (unsigned char *)dec_dept);
        if (dec_len < 0) {
            strcpy(dec_dept, "[DECRYPT ERROR]");
        } else {
            dec_dept[dec_len] = '\0';
        }

        printf("%-6d %-20s %-20s %.64s\n",
               p.id, dec_name, dec_dept, p.password_hash);
        count++;
    }

    fclose(fp);

    if (count == 0)
        printf("No records found.\n");
}


void verifyPassword(void) {
    struct Personnel p;
    FILE *fp;
    char line[64];
    char raw_pass[50];
    char check_hash[65];
    int target_id;
    int found = 0;

    printf("\n--- Verify Password ---\n");

    printf("Enter Record ID: ");
    fflush(stdout);
    if (fgets(line, sizeof(line), stdin) == NULL) return;
    if (sscanf(line, "%d", &target_id) != 1) {
        printf("Invalid ID.\n");
        return;
    }

    printf("Enter Password : ");
    fflush(stdout);
    if (fgets(raw_pass, sizeof(raw_pass), stdin) == NULL) return;
    if (strchr(raw_pass, '\n') == NULL)
        drainStdin();
    raw_pass[strcspn(raw_pass, "\n")] = '\0';

    /* hash what the user typed */
    sha256_hash(raw_pass, check_hash);
    memset(raw_pass, 0, sizeof(raw_pass)); /* scrub plaintext */

    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records file found.\n");
        return;
    }

    while (fread(&p, sizeof(struct Personnel), 1, fp) == 1) {
        if (p.id == target_id) {
            found = 1;
            if (strcmp(p.password_hash, check_hash) == 0) {
                printf("Password MATCHES for record %d.\n", target_id);
            } else {
                printf("Password does NOT match for record %d.\n", target_id);
            }
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("Record with ID %d not found.\n", target_id);
}
