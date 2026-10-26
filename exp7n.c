#include <stdio.h>
#include <stdbool.h>

#define MAX_VMS 10
#define MAX_RESOURCES 10

int main() {
    int n, m;
    int alloc[MAX_VMS][MAX_RESOURCES];
    int request[MAX_VMS][MAX_RESOURCES];
    int avail[MAX_RESOURCES];
    int work[MAX_RESOURCES];
    bool finish[MAX_VMS] = {false};

    printf("--- Cloud Resource Manager: Deadlock Detection ---\n");
    printf("Enter number of Virtual Machines (VMs): ");
    scanf("%d", &n);
    printf("Enter number of Cloud Resource Types: ");
    scanf("%d", &m);

    printf("\nEnter Currently Allocated Resources for each VM:\n");
    for (int i = 0; i < n; i++) {
        printf("VM %d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &alloc[i][j]);
    }

    printf("\nEnter Pending Requests from each VM:\n");
    for (int i = 0; i < n; i++) {
        printf("VM %d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &request[i][j]);
    }

    printf("\nEnter Available Cloud Resources: ");
    for (int j = 0; j < m; j++) {
        scanf("%d", &avail[j]);
        work[j] = avail[j];
    }

    // Mark VMs with no allocation as finished
    for (int i = 0; i < n; i++) {
        bool hasAllocation = false;
        for (int j = 0; j < m; j++) {
            if (alloc[i][j] != 0) {
                hasAllocation = true;
                break;
            }
        }
        if (!hasAllocation) finish[i] = true;
    }

    // Algorithm to check for unresolvable requests
    int count = 0;
    while (count < n) {
        bool found = false;
        for (int p = 0; p < n; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < m; j++)
                    if (request[p][j] > work[j]) break;

                if (j == m) { // VM request can be satisfied
                    for (int k = 0; k < m; k++) work[k] += alloc[p][k];
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) break; // Cannot proceed further
    }

    // Check if any VM couldn't finish
    bool deadlocked = false;
    printf("\n--- Result ---\n");
    for (int i = 0; i < n; i++) {
        if (!finish[i]) {
            if (!deadlocked) {
                printf("[DEADLOCK DETECTED] The following cloud VMs are deadlocked:\n");
                deadlocked = true;
            }
            printf("- VM %d\n", i);
        }
    }

    if (!deadlocked) {
        printf("[CLEAR] No deadlock detected. All cloud tasks can complete safely.\n");
    }

    return 0;
}
