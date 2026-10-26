#include <stdio.h>

void allocate(int b[], int m, int p[], int n, int is_worst) {
    int occ[20] = {0};
    printf("\n--- %s Fit ---\nProc\tSize\tBlock\n", is_worst ? "Worst" : "Best");
    
    for (int i = 0; i < n; i++) {
        int idx = -1;
        for (int j = 0; j < m; j++) {
            if (b[j] >= p[i] && !occ[j]) {
                if (idx == -1 || (is_worst ? b[j] > b[idx] : b[j] < b[idx]))
                    idx = j;
            }
        }
        if (idx != -1) { occ[idx] = 1; printf("%d\t%d\tBlock %d\n", i + 1, p[i], idx + 1); }
        else printf("%d\t%d\tNot Allocated\n", i + 1, p[i]);
    }
}

int main() {
    int m, n, b[20], p[20];

    printf("Blocks count & sizes: ");
    scanf("%d", &m);
    for (int i = 0; i < m; i++) scanf("%d", &b[i]);

    printf("Processes count & sizes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);

    allocate(b, m, p, n, 0); // Best Fit
    allocate(b, m, p, n, 1); // Worst Fit

    return 0;
}
