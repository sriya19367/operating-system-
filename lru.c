#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 50
#define RAM_SLOTS 3 // Total available RAM slots for apps

typedef struct {
    char name[MAX_NAME];
    int last_used_time; // Counter tracking when the app was last accessed
} RamSlot;

typedef struct {
    char name[MAX_NAME];
} AppRequest;

// Convert string to lowercase for case-insensitive matching (e.g., 'WhatsApp' == 'whatsapp')
void to_lower_str(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Check if an app is already inside RAM
int find_in_ram(RamSlot ram[], int capacity, const char *app_name) {
    char target[MAX_NAME];
    strncpy(target, app_name, MAX_NAME - 1);
    target[MAX_NAME - 1] = '\0';
    to_lower_str(target);

    for (int i = 0; i < capacity; i++) {
        if (strlen(ram[i].name) > 0) {
            char current[MAX_NAME];
            strncpy(current, ram[i].name, MAX_NAME - 1);
            current[MAX_NAME - 1] = '\0';
            to_lower_str(current);

            if (strcmp(current, target) == 0) {
                return i; // Found in RAM at slot index i
            }
        }
    }
    return -1; // Not in RAM
}

// Find the slot index of the Least Recently Used (LRU) app
int find_lru_index(RamSlot ram[], int capacity) {
    int min_time = ram[0].last_used_time;
    int lru_index = 0;

    for (int i = 1; i < capacity; i++) {
        if (ram[i].last_used_time < min_time) {
            min_time = ram[i].last_used_time;
            lru_index = i;
        }
    }
    return lru_index;
}

// Display current RAM state
void print_ram_state(RamSlot ram[], int capacity) {
    printf("[ ");
    for (int i = 0; i < capacity; i++) {
        if (strlen(ram[i].name) > 0) {
            printf("%s ", ram[i].name);
        } else {
            printf("--- ");
        }
    }
    printf("]");
}

int main() {
    int num_apps;

    printf("========================================================\n");
    printf("     MOBILE MEMORY MANAGEMENT (LRU PAGE REPLACEMENT)    \n");
    printf("========================================================\n");
    printf("System Specs: Physical RAM Capacity = %d App Slots\n\n", RAM_SLOTS);

    // 1. Get total number of app requests
    printf("Enter number of app launch requests: ");
    if (scanf("%d", &num_apps) != 1 || num_apps <= 0) {
        printf("Invalid input.\n");
        return 1;
    }

    AppRequest *requests = (AppRequest *)malloc(num_apps * sizeof(AppRequest));
    if (!requests) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    getchar(); // Clear newline buffer

    // 2. Collect app launch sequence
    printf("\n--- ENTER APP LAUNCH SEQUENCE ---\n");
    for (int i = 0; i < num_apps; i++) {
        printf("Enter name for App request %d (e.g., WhatsApp, Camera, Instagram): ", i + 1);
        fgets(requests[i].name, sizeof(requests[i].name), stdin);
        requests[i].name[strcspn(requests[i].name, "\n")] = 0; // Strip trailing newline
    }

    // Initialize RAM slots
    RamSlot ram[RAM_SLOTS];
    for (int i = 0; i < RAM_SLOTS; i++) {
        ram[i].name[0] = '\0';
        ram[i].last_used_time = -1;
    }

    int page_faults = 0;
    int page_hits = 0;
    int occupied_slots = 0;

    // 3. Process requests with LRU policy
    printf("\n========================================================================================\n");
    printf("                                MEMORY MANAGEMENT LOG                                   \n");
    printf("========================================================================================\n");
    printf("%-5s %-15s %-18s %-25s %-15s\n", "Step", "App Opened", "Status", "RAM State [Slot1, Slot2, Slot3]", "Action Taken");
    printf("----------------------------------------------------------------------------------------\n");

    for (int step = 1; step <= num_apps; step++) {
        char *current_app = requests[step - 1].name;
        int hit_index = find_in_ram(ram, RAM_SLOTS, current_app);

        if (hit_index != -1) {
            // PAGE HIT: App is already inside RAM
            page_hits++;
            ram[hit_index].last_used_time = step; // Refresh usage timestamp to current step

            printf("%-5d %-15s %-18s ", step, current_app, "HIT (In RAM)");
            print_ram_state(ram, RAM_SLOTS);
            printf(" %-15s\n", "Refreshed LRU");
        } else {
            // PAGE FAULT: App not in RAM
            page_faults++;
            char action_msg[100];

            if (occupied_slots < RAM_SLOTS) {
                // Free slot available
                strcpy(ram[occupied_slots].name, current_app);
                ram[occupied_slots].last_used_time = step;
                snprintf(action_msg, sizeof(action_msg), "Loaded into Slot %d", occupied_slots + 1);
                occupied_slots++;
            } else {
                // RAM full: Find and evict Least Recently Used (LRU) app
                int lru_index = find_lru_index(ram, RAM_SLOTS);
                snprintf(action_msg, sizeof(action_msg), "Evicted %s (LRU)", ram[lru_index].name);

                strcpy(ram[lru_index].name, current_app);
                ram[lru_index].last_used_time = step;
            }

            printf("%-5d %-15s %-18s ", step, current_app, "FAULT (Loaded)");
            print_ram_state(ram, RAM_SLOTS);
            printf(" %-15s\n", action_msg);
        }
    }

    // 4. Output Summary
    printf("----------------------------------------------------------------------------------------\n");
    printf("Total App Requests : %d\n", num_apps);
    printf("Page Faults (Misses): %d (Loaded cold from storage)\n", page_faults);
    printf("Page Hits          : %d (Resumed instantly from RAM)\n", page_hits);
    printf("Hit Ratio          : %.2f%%\n", ((float)page_hits / num_apps) * 100);
    printf("Fault Ratio        : %.2f%%\n", ((float)page_faults / num_apps) * 100);
    printf("========================================================================================\n");

    free(requests);
    return 0;
}
