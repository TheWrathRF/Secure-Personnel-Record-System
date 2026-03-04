
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
void xorEncryptDecrypt(char *data, char key);


int main(void) {
    int choice;

    do {
        printf("\n===================================\n");
        printf("  Secure Personnel Record System\n");
        printf("===================================\n");
        printf("  1. Add Record\n");
        printf("  2. View Records\n");
        printf("  3. Exit\n");
        printf("===================================\n");
        printf("  Enter your choice: ");
        scanf("%d", &choice);

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


void xorEncryptDecrypt(char *data, char key) {
    int i;
    for (i = 0; data[i] != '\0'; i++) {
        data[i] = data[i] ^ key;
    }
}


void addRecord(void) {
    struct Personnel p;
    FILE *fp;

    printf("\n--- Add New Record ---\n");

    printf("Enter ID       : ");
    scanf("%d", &p.id);
    getchar(); 

    printf("Enter Name     : ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    printf("Enter Dept     : ");
    fgets(p.dept, sizeof(p.dept), stdin);
    p.dept[strcspn(p.dept, "\n")] = '\0';

    printf("Enter Password : ");
    fgets(p.password, sizeof(p.password), stdin);
    p.password[strcspn(p.password, "\n")] = '\0';

    
    xorEncryptDecrypt(p.password, XOR_KEY);

    fp = fopen(DATA_FILE, "ab");
    if (fp == NULL) {
        perror("Error: Could not open file for writing");
        return;
    }

    fwrite(&p, sizeof(struct Personnel), 1, fp);
    fclose(fp);

    printf("Record saved successfully! (password is encrypted)\n");
}


void viewRecords(void) {
    struct Personnel p;
    FILE *fp;
    int count = 0;

    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        perror("Error: Could not open file for reading");
        printf("No records file found. Add a record first.\n");
        return;
    }

    printf("\n%-6s %-20s %-20s %-20s\n",
           "ID", "Name", "Department", "Password");
    printf("--------------------------------------------------------------\n");

    while (fread(&p, sizeof(struct Personnel), 1, fp) == 1) {
        
        xorEncryptDecrypt(p.password, XOR_KEY);

        printf("%-6d %-20s %-20s %-20s\n",
               p.id, p.name, p.dept, p.password);
        count++;
    }

    fclose(fp);

    if (count == 0)
        printf("No records found.\n");
}
