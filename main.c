
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Personnel {
    int id;
    char name[50];
    char dept[50];
    char password[50];
};

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
                printf("\n[Add Record] Coming soon...\n");
                break;
            case 2:
                printf("\n[View Records] Coming soon...\n");
                break;
            case 3:
                printf("\nExiting. Goodbye!\n");
                break;
        }

    } while (choice != 3);

    return 0;
}
