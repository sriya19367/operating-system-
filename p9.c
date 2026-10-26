#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char name[50];
    int burst_time;
    int priority; // Lower value = higher execution priority
    int waiting_time;
    int turnaround_time;
} GameTask;

// Helper to convert string to lowercase for easy matching
void to_lower_str(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Assigns priority based on simple/basic task names
int assign_priority(const char *raw_name) {
    char name[50];
    strncpy(name, raw_name, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    to_lower_str(name);

    if (strstr(name, "hit") || strstr(name, "move") || strstr(name, "shoot") || strstr(name, "jump")) {
        return 1; // Priority 1 (Highest): Real-time gameplay actions (hit, move, shoot)
    } else if (strstr(name, "score") || strstr(name, "rank") || strstr(name, "health") || strstr(name, "kill")) {
        return 2; // Priority 2: Game state updates (score, rank, health)
    } else if (strstr(name, "chat") || strstr(name, "msg") || strstr(name, "voice")) {
        return 3; // Priority 3: Communication features (chat, messaging)
    } else if (strstr(name, "spawn") || strstr(name, "loot") || strstr(name, "npc")) {
        return 4; // Priority 4: Environment/world events (spawn, loot)
    } else {
        return 5; // Priority 5 (Lowest): Background processes (save, log, settings)
    }
}

// Sort tasks by priority (Non-preemptive priority scheduling)
void sort_by_priority(GameTask tasks[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (tasks[j].priority > tasks[j + 1].priority) {
                GameTask temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }
}

// Calculate waiting and turnaround times
void calculate_times(GameTask tasks[], int n) {
    tasks[0].waiting_time = 0;
    tasks[0].turnaround_time = tasks[0].burst_time;

    for (int i = 1; i < n; i++) {
        tasks[i].waiting_time = tasks[i - 1].waiting_time + tasks[i - 1].burst_time;
        tasks[i].turnaround_time = tasks[i].waiting_time + tasks[i].burst_time;
    }
}

int main() {
    int n;

    printf("===================================================\n");
    printf("   MULTIPLAYER GAME SERVER TASK PRIORITY SCHEDULER  \n");
    printf("===================================================\n\n");

    // 1. Get total number of tasks
    printf("Enter total number of tasks: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input for number of tasks.\n");
        return 1;
    }

    GameTask *tasks = (GameTask *)malloc(n * sizeof(GameTask));
    if (!tasks) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    getchar(); // Clear buffer newline

    // 2. Collect ALL tasks first (Batch Entry)
    printf("\n--- ENTER ALL TASK DETAILS ---\n");
    for (int i = 0; i < n; i++) {
        tasks[i].id = i + 1;

        printf("\nTask %d Name (e.g., hit, score, chat, save): ", i + 1);
        fgets(tasks[i].name, sizeof(tasks[i].name), stdin);
        tasks[i].name[strcspn(tasks[i].name, "\n")] = 0; // Strip newline

        printf("Task %d Burst Time (ms): ", i + 1);
        scanf("%d", &tasks[i].burst_time);
        getchar(); // Clear buffer newline
    }

    // 3. Assign priorities after all tasks are entered
    for (int i = 0; i < n; i++) {
        tasks[i].priority = assign_priority(tasks[i].name);
    }

    // 4. Display assigned priorities summary
    printf("\n===================================================\n");
    printf("              ASSIGNED TASK PRIORITIES             \n");
    printf("===================================================\n");
    for (int i = 0; i < n; i++) {
        printf("Task %d: %-15s -> Priority Assigned: %d ", tasks[i].id, tasks[i].name, tasks[i].priority);
        switch (tasks[i].priority) {
            case 1: printf("(Level 1: Critical Gameplay - hit/move)\n"); break;
            case 2: printf("(Level 2: Game State - score/rank)\n"); break;
            case 3: printf("(Level 3: Communication - chat/msg)\n"); break;
            case 4: printf("(Level 4: World Logic - spawn/loot)\n"); break;
            default: printf("(Level 5: Background - save/log)\n"); break;
        }
    }

    // 5. Sort by priority and compute scheduling times
    sort_by_priority(tasks, n);
    calculate_times(tasks, n);

    // 6. Display final execution schedule table
    printf("\n========================================================================================\n");
    printf("                                  EXECUTION SCHEDULE                                    \n");
    printf("========================================================================================\n");
    printf("%-8s %-20s %-10s %-12s %-12s %-15s\n", 
           "Task ID", "Task Name", "Priority", "Burst (ms)", "Wait (ms)", "Turnaround (ms)");
    printf("----------------------------------------------------------------------------------------\n");

    float total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        total_wt += tasks[i].waiting_time;
        total_tat += tasks[i].turnaround_time;
        printf("%-8d %-20s %-10d %-12d %-12d %-15d\n",
               tasks[i].id, tasks[i].name, tasks[i].priority, 
               tasks[i].burst_time, tasks[i].waiting_time, tasks[i].turnaround_time);
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("Average Waiting Time    : %.2f ms\n", total_wt / n);
    printf("Average Turnaround Time : %.2f ms\n", total_tat / n);
    printf("========================================================================================\n");

    free(tasks);
    return 0;
}
