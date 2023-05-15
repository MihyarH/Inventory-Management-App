#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define STORAGE_FILE "storage.txt"

struct StorageItem {
    char name[MAX_NAME_LEN];
    int quantity;
    struct StorageItem* next;
};

void getInput(char* name, int* quantity) {
    printf("Enter the name of the item: ");
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';  // Remove trailing newline character

    printf("Enter the quantity of the item: ");
    scanf("%d", quantity);
    getchar();  // Consume the newline character from previous input
}

void addItem(struct StorageItem** head, char* name, int quantity) {
    struct StorageItem* newItem = malloc(sizeof(struct StorageItem));
    strncpy(newItem->name, name, MAX_NAME_LEN);
    newItem->quantity = quantity;
    newItem->next = *head;
    *head = newItem;
}

void deleteItem(struct StorageItem** head, char* name) {
    struct StorageItem* current = *head;
    struct StorageItem* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Item '%s' removed from storage\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Item '%s' not found in storage\n", name);
}

void updateItem(struct StorageItem* head, char* name, int quantity) {
    struct StorageItem* current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->quantity = quantity;
            printf("Quantity of item '%s' updated to %d\n", name, quantity);
            return;
        }
        current = current->next;
    }

    printf("Item '%s' not found in storage\n", name);
}

void printItems(struct StorageItem* head) {
    if (head == NULL) {
        printf("There are no items in the storage\n");
        return;
    }

    printf("Items in the storage:\n");
    struct StorageItem* current = head;

    FILE* file = fopen(STORAGE_FILE, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", STORAGE_FILE);
        return;
    }

    while (current != NULL) {
        printf("%s: %d\n", current->name, current->quantity);
        fprintf(file, "%s %d\n", current->name, current->quantity);
        current = current->next;
    }

    fclose(file);
}

void loadItemsFromFile(struct StorageItem** head, const char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[MAX_NAME_LEN + 10];  // Increased buffer size for reading lines
    char name[MAX_NAME_LEN];
    int quantity;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%s %d", name, &quantity) == 2) {
            addItem(head, name, quantity);
        }
    }

    fclose(file);
}

void saveItemsToFile(struct StorageItem *head, const char* filename) {
    FILE* file;
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    struct StorageItem* current = head;
    while (current != NULL) {
        fprintf(file, "%s %d\n", current->name, current->quantity);
        current = current->next;
    }

    fclose(file);
}

void freeStorage(struct StorageItem* head) {
    struct StorageItem* current = head;
    while (current != NULL) {
        struct StorageItem* temp = current;
        current = current->next;
        free(temp);
    }
}

void displayMenu() {
    printf("1. Add item\n");
    printf("2. Remove item\n");
    printf("3. Update item quantity\n");
    printf("4. Print items\n");
    printf("5. Exit\n");
}

int getUserChoice() {
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();  // Consume the newline character from input
    return choice;
}

void executeChoice(struct StorageItem** head, int choice) {
    char name[MAX_NAME_LEN];
    int quantity;

    switch (choice) {
        case 1:
            getInput(name, &quantity);
            addItem(head, name, quantity);
            break;
        case 2:
            printf("Enter the name of the item to remove: ");
            fgets(name, MAX_NAME_LEN, stdin);
            name[strcspn(name, "\n")] = '\0';  // Remove trailing newline character
            deleteItem(head, name);
            break;
        case 3:
            printf("Enter the name of the item to update: ");
            fgets(name, MAX_NAME_LEN, stdin);
            name[strcspn(name, "\n")] = '\0';  // Remove trailing newline character
            printf("Enter the new quantity of the item: ");
            scanf("%d", &quantity);
            getchar();  // Consume the newline character from input
            updateItem(*head, name, quantity);
            break;
        case 4:
            printItems(*head);
            break;
        case 5:
            saveItemsToFile(*head, STORAGE_FILE);
            freeStorage(*head);
            exit(0);
        default:
            printf("Invalid choice\n");
    }
}

int main() {
    struct StorageItem* head = NULL;
    loadItemsFromFile(&head, STORAGE_FILE);

    while (1) {
        displayMenu();
        int choice = getUserChoice();
        executeChoice(&head, choice);
    }

    return 0;
}

