

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "records.dat"


struct Personnel {
    int id;
    char name[50];
    char dept[50];
    char password[50];
};


void addRecord(void);
void viewRecords(void);


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
        }

    } while (choice != 3);

    return 0;
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

    fp = fopen(DATA_FILE, "ab");
    fwrite(&p, sizeof(struct Personnel), 1, fp);
    fclose(fp);

    printf("Record saved successfully!\n");
}


void viewRecords(void) {
    struct Personnel p;
    FILE *fp;
    int count = 0;

    fp = fopen(DATA_FILE, "rb");

    printf("\n%-6s %-20s %-20s %-20s\n",
           "ID", "Name", "Department", "Password");
    printf("--------------------------------------------------------------\n");

    while (fread(&p, sizeof(struct Personnel), 1, fp) == 1) {
        printf("%-6d %-20s %-20s %-20s\n",
               p.id, p.name, p.dept, p.password);
        count++;
    }

    fclose(fp);

    if (count == 0)
        printf("No records found.\n");
}
