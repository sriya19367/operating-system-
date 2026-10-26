#include <stdio.h>

void worstFit(int b[], int m, int p[], int n) {
    int occ[20] = {0};
    printf("\n--- Worst Fit Allocation Result ---\nProcess No.\tProcess Size\tBlock No.\n");
    
    for (int i = 0; i < n; i++) {
        int w = -1;
        for (int j = 0; j < m; j++) {
            if (b[j] >= p[i] && !occ[j] && (w == -1 || b[j] > b[w]))
                w = j;
        }
        printf(" %d\t\t%d\t\t", i + 1, p[i]);
        if (w != -1) { occ[w] = 1; printf("%d\n", w + 1); }
        else printf("Not Allocated\n");
    }
}

int main() {
    int m, n, b[20], p[20];

    printf("Enter total number of memory blocks: ");
    scanf("%d", &m);
    printf("Enter size of each memory block:\n");
    for (int i = 0; i < m; i++) { printf("Block %d: ", i + 1); scanf("%d", &b[i]); }

    printf("\nEnter total number of processes: ");
    scanf("%d", &n);
    printf("Enter size of each process:\n");
    for (int i = 0; i < n; i++) { printf("Process %d: ", i + 1); scanf("%d", &p[i]); }

    worstFit(b, m, p, n);
    return 0;
}
