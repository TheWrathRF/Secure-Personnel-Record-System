#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "records.dat"
#define XOR_KEY   'K'


struct Personnel {
    int id;
    char name[50];
    char dept[50];
    char password[50];
};


void addRecord(void);
void viewRecords(void);
void xorEncryptDecrypt(char *data, char key, int len);
void drainStdin(void);


int main(void) {
    int choice = 0;        
    char buffer[64];       

    do {
        printf("\n===================================\n");
        printf("  Secure Personnel Record System\n");
        printf("===================================\n");
        printf("  1. Add Record\n");
        printf("  2. View Records\n");
        printf("  3. Exit\n");
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
                printf("\nExiting. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }

    } while (choice != 3);

    return 0;
}



void drainStdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}


void xorEncryptDecrypt(char *data, char key, int len) {
    int i;
    for (i = 0; i < len; i++) {
        data[i] = data[i] ^ key;
    }
}


void addRecord(void) {
    struct Personnel p;
    FILE *fp;
    char line[64];

    memset(&p, 0, sizeof(p));

    printf("\n--- Add New Record ---\n");


    printf("Enter ID       : ");
    fflush(stdout);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Input ended. Record not saved.\n");
        return;
    }
    if (sscanf(line, "%d", &p.id) != 1 || p.id <= 0) {
        printf("Invalid ID (must be a positive number). Record not saved.\n");
        return;
    }


    printf("Enter Name     : ");
    fflush(stdout);
    if (fgets(p.name, sizeof(p.name), stdin) == NULL) {
        printf("Input ended. Record not saved.\n");
        return;
    }
    /* If no newline was read, input was too long — drain the rest ( Could be static inline later) */
    if (strchr(p.name, '\n') == NULL) {
        drainStdin();
    }
    p.name[strcspn(p.name, "\n")] = '\0';

    if (strlen(p.name) == 0) {
        printf("Name cannot be empty. Record not saved.\n");
        return;
    }


    printf("Enter Dept     : ");
    fflush(stdout);
    if (fgets(p.dept, sizeof(p.dept), stdin) == NULL) {
        printf("Input ended. Record not saved.\n");
        return;
    }
    if (strchr(p.dept, '\n') == NULL) {
        drainStdin();
    }
    p.dept[strcspn(p.dept, "\n")] = '\0';

    if (strlen(p.dept) == 0) {
        printf("Department cannot be empty. Record not saved.\n");
        return;
    }


    printf("Enter Password : ");
    fflush(stdout);
    if (fgets(p.password, sizeof(p.password), stdin) == NULL) {
        printf("Input ended. Record not saved.\n");
        return;
    }
    if (strchr(p.password, '\n') == NULL) {
        drainStdin();
    }
    p.password[strcspn(p.password, "\n")] = '\0';

    if (strlen(p.password) == 0) {
        printf("Password cannot be empty. Record not saved.\n");
        return;
    }

    xorEncryptDecrypt(p.password, XOR_KEY, sizeof(p.password));

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

    printf("Record saved successfully! (password is encrypted)\n");
}


void viewRecords(void) {
    struct Personnel p;
    FILE *fp;
    int count = 0;

    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No records file found. Add a record first.\n");
        return;
    }

    printf("\n%-6s %-20s %-20s %-20s\n",
           "ID", "Name", "Department", "Password");
    printf("--------------------------------------------------------------\n");

    while (fread(&p, sizeof(struct Personnel), 1, fp) == 1) {
        xorEncryptDecrypt(p.password, XOR_KEY, sizeof(p.password));

        printf("%-6d %-20s %-20s %-20s\n",
               p.id, p.name, p.dept, p.password);
        count++;
    }

    fclose(fp);

    if (count == 0)
        printf("No records found.\n");
}
