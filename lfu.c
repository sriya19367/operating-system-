#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 50
#define RAM_SLOTS 3 // Total available RAM slots for apps

typedef struct {
    char name[MAX_NAME];
    int access_count; // Tracks how many times the app was accessed/opened
    int load_time;    // Tracks when the app was placed in RAM (used for FIFO tie-breaker)
} RamSlot;

typedef struct {
    char name[MAX_NAME];
} AppRequest;

// Convert string to lowercase for case-insensitive matching
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

// Find slot index of the Least Frequently Used (LFU) app (with FIFO tie-breaking)
int find_lfu_index(RamSlot ram[], int capacity) {
    int min_freq = ram[0].access_count;
    int min_time = ram[0].load_time;
    int lfu_index = 0;

    for (int i = 1; i < capacity; i++) {
        if (ram[i].access_count < min_freq) {
            min_freq = ram[i].access_count;
            min_time = ram[i].load_time;
            lfu_index = i;
        } else if (ram[i].access_count == min_freq) {
            // Tie-breaker: If frequencies match, evict the one loaded earliest (FIFO)
            if (ram[i].load_time < min_time) {
                min_time = ram[i].load_time;
                lfu_index = i;
            }
        }
    }
    return lfu_index;
}

// Display current RAM state
void print_ram_state(RamSlot ram[], int capacity) {
    printf("[ ");
    for (int i = 0; i < capacity; i++) {
        if (strlen(ram[i].name) > 0) {
            printf("%s(x%d) ", ram[i].name, ram[i].access_count);
        } else {
            printf("--- ");
        }
    }
    printf("]");
}

int main() {
    int num_apps;

    printf("========================================================\n");
    printf("     MOBILE MEMORY MANAGEMENT (LFU PAGE REPLACEMENT)    \n");
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
        ram[i].access_count = 0;
        ram[i].load_time = -1;
    }

    int page_faults = 0;
    int page_hits = 0;
    int occupied_slots = 0;

    // 3. Process requests with LFU policy
    printf("\n========================================================================================\n");
    printf("                                MEMORY MANAGEMENT LOG                                   \n");
    printf("========================================================================================\n");
    printf("%-5s %-15s %-18s %-32s %-15s\n", "Step", "App Opened", "Status", "RAM State [App(Frequency)]", "Action Taken");
    printf("----------------------------------------------------------------------------------------\n");

    for (int step = 1; step <= num_apps; step++) {
        char *current_app = requests[step - 1].name;
        int hit_index = find_in_ram(ram, RAM_SLOTS, current_app);

        if (hit_index != -1) {
            // PAGE HIT: App is already in RAM -> increment its access counter
            page_hits++;
            ram[hit_index].access_count++;

            printf("%-5d %-15s %-18s ", step, current_app, "HIT (In RAM)");
            print_ram_state(ram, RAM_SLOTS);
            printf(" %-15s\n", "Freq Incremented");
        } else {
            // PAGE FAULT: App not in RAM
            page_faults++;
            char action_msg[100];

            if (occupied_slots < RAM_SLOTS) {
                // Free slot available
                strcpy(ram[occupied_slots].name, current_app);
                ram[occupied_slots].access_count = 1;
                ram[occupied_slots].load_time = step;
                snprintf(action_msg, sizeof(action_msg), "Loaded into Slot %d", occupied_slots + 1);
                occupied_slots++;
            } else {
                // RAM full: Find and evict Least Frequently Used (LFU) app
                int lfu_index = find_lfu_index(ram, RAM_SLOTS);
                snprintf(action_msg, sizeof(action_msg), "Evicted %s (LFU)", ram[lfu_index].name);

                strcpy(ram[lfu_index].name, current_app);
                ram[lfu_index].access_count = 1;
                ram[lfu_index].load_time = step;
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
