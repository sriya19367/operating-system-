#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 10
#define MAX_FILES 20
#define MAX_NAME_LEN 50

// Level 2: User/Student Directory Structure
struct UserDirectory {
    char student_name[MAX_NAME_LEN];
    int num_files;
    char files[MAX_FILES][MAX_NAME_LEN];
};

int main() {
    int n, i, j;
    struct UserDirectory master_directory[MAX_STUDENTS];

    printf("Enter number of students in the workshop: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_STUDENTS) {
        printf("Invalid number of students.\n");
        return 1;
    }

    // Input Phase: Collecting user-specific files (Level 2 directories)
    for (i = 0; i < n; i++) {
        snprintf(master_directory[i].student_name, MAX_NAME_LEN, "Student %d", i + 1);
        
        printf("%s - Number of files: ", master_directory[i].student_name);
        scanf("%d", &master_directory[i].num_files);

        for (j = 0; j < master_directory[i].num_files; j++) {
            printf("Enter file name: ");
            scanf("%s", master_directory[i].files[j]);
        }
    }

    printf("\n");

    // Output Phase: Displaying the Two-Level Directory Structure
    for (i = 0; i < n; i++) {
        printf("%s Directory:\n", master_directory[i].student_name);
        for (j = 0; j < master_directory[i].num_files; j++) {
            printf("%s\n", master_directory[i].files[j]);
        }
        printf("\n");
    }

    return 0;
}