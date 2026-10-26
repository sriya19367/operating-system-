#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int id;
    int processing_time; // Total time needed for checkout
    int remaining_time;  // Remaining processing time
    int arrival_time;    // Arrival order time
    int completion_time;
    int waiting_time;
    int turnaround_time;
} Customer;

int main() {
    int n, time_quantum;

    printf("========================================================\n");
    printf("   E-COMMERCE CHECKOUT SYSTEM - ROUND ROBIN SCHEDULER   \n");
    printf("========================================================\n\n");

    // 1. Get total number of customers
    printf("Enter number of customers: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of customers.\n");
        return 1;
    }

    Customer *customers = (Customer *)malloc(n * sizeof(Customer));
    if (!customers) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // 2. Collect Processing Time for each customer
    printf("\n--- ENTER CUSTOMER CHECKOUT DETAILS ---\n");
    for (int i = 0; i < n; i++) {
        customers[i].id = i + 1;
        customers[i].arrival_time = 0; // All customers arrive at start of checkout burst
        
        printf("Customer %d Processing Time (ms): ", i + 1);
        scanf("%d", &customers[i].processing_time);
        
        customers[i].remaining_time = customers[i].processing_time;
    }

    // 3. Get Time Quantum
    printf("\nEnter Time Quantum (ms): ");
    if (scanf("%d", &time_quantum) != 1 || time_quantum <= 0) {
        printf("Invalid time quantum.\n");
        free(customers);
        return 1;
    }

    // 4. Perform Round Robin Scheduling
    int current_time = 0;
    int completed = 0;

    printf("\n========================================================\n");
    printf("                  CHECKOUT EXECUTION LOG                \n");
    printf("========================================================\n");

    while (completed < n) {
        bool idle = true;

        for (int i = 0; i < n; i++) {
            if (customers[i].remaining_time > 0) {
                idle = false;

                if (customers[i].remaining_time > time_quantum) {
                    printf("Time %2d ms - %2d ms : Customer %d processed for %d ms (Remaining: %d ms)\n",
                           current_time, current_time + time_quantum, customers[i].id, 
                           time_quantum, customers[i].remaining_time - time_quantum);
                    
                    current_time += time_quantum;
                    customers[i].remaining_time -= time_quantum;
                } else {
                    printf("Time %2d ms - %2d ms : Customer %d finished checkout (%d ms)\n",
                           current_time, current_time + customers[i].remaining_time, 
                           customers[i].id, customers[i].remaining_time);
                    
                    current_time += customers[i].remaining_time;
                    customers[i].remaining_time = 0;
                    customers[i].completion_time = current_time;
                    
                    // Turnaround Time = Completion Time - Arrival Time
                    customers[i].turnaround_time = customers[i].completion_time - customers[i].arrival_time;
                    // Waiting Time = Turnaround Time - Processing Time
                    customers[i].waiting_time = customers[i].turnaround_time - customers[i].processing_time;
                    
                    completed++;
                }
            }
        }

        if (idle) {
            break;
        }
    }

    // 5. Display Summary Output
    printf("\n========================================================================================\n");
    printf("                                CHECKOUT SUMMARY REPORT                                 \n");
    printf("========================================================================================\n");
    printf("%-12s %-20s %-15s %-15s %-18s\n", 
           "Customer ID", "Processing (ms)", "Completion (ms)", "Wait Time (ms)", "Turnaround (ms)");
    printf("----------------------------------------------------------------------------------------\n");

    float total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        total_wt += customers[i].waiting_time;
        total_tat += customers[i].turnaround_time;
        
        printf("%-12d %-20d %-15d %-15d %-18d\n",
               customers[i].id, customers[i].processing_time, 
               customers[i].completion_time, customers[i].waiting_time, customers[i].turnaround_time);
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("Time Quantum Used       : %d ms\n", time_quantum);
    printf("Average Waiting Time    : %.2f ms\n", total_wt / n);
    printf("Average Turnaround Time : %.2f ms\n", total_tat / n);
    printf("========================================================================================\n");

    free(customers);
    return 0;
}
