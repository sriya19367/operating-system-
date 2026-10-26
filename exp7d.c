#include <stdio.h>
#include <stdbool.h>

#define MAX_VMS 10
#define MAX_RESOURCES 10

int n, m; // Number of VMs and Cloud Resources
int alloc[MAX_VMS][MAX_RESOURCES];
int max[MAX_VMS][MAX_RESOURCES];
int need[MAX_VMS][MAX_RESOURCES];
int avail[MAX_RESOURCES];

void calculateNeed() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

bool isSafeSequence(int safeSeq[]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_VMS] = {false};

    for (int i = 0; i < m; i++) {
        work[i] = avail[i];
    }

    int count = 0;
    while (count < n) {
        bool found = false;
        for (int p = 0; p < n; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < m; j++) {
                    if (need[p][j] > work[j])
                        break;
                }

                if (j == m) { // All resource needs satisfied for VM p
                    for (int k = 0; k < m; k++) {
                        work[k] += alloc[p][k];
                    }
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            return false; // System is in unsafe state
        }
    }
    return true;
}

int main() {
    printf("--- Cloud Resource Manager: Banker's Algorithm ---\n");
    printf("Enter number of Virtual Machines (VMs): ");
    scanf("%d", &n);
    printf("Enter number of Cloud Resource Types (e.g., CPU, RAM): ");
    scanf("%d", &m);

    printf("\nEnter Currently Allocated Resources for each VM:\n");
    for (int i = 0; i < n; i++) {
        printf("VM %d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &alloc[i][j]);
    }

    printf("\nEnter Maximum Resource Demand for each VM:\n");
    for (int i = 0; i < n; i++) {
        printf("VM %d: ", i);
        for (int j = 0; j < m; j++) scanf("%d", &max[i][j]);
    }

    printf("\nEnter Available Cloud Resources: ");
    for (int j = 0; j < m; j++) scanf("%d", &avail[j]);

    calculateNeed();

    int safeSeq[MAX_VMS];
    if (isSafeSequence(safeSeq)) {
        printf("\n[SUCCESS] System is in a SAFE state!\nSafe Execution Order of VMs: ");
        for (int i = 0; i < n; i++) {
            printf("VM %d ", safeSeq[i]);
        }
        printf("\nNo deadlock will occur.\n");
    } else {
        printf("\n[WARNING] System is in an UNSAFE state! Deadlock could occur.\n");
    }

    return 0;
}
