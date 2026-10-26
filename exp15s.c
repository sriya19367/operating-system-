#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_NAME 100
#define DIRECTORY_NAME "camera_dir"

// Function to create the directory if it doesn't exist
void ensure_directory_exists() {
    // Using system command or mkdir to keep it simple and portable for Linux
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", DIRECTORY_NAME);
    system(cmd);
}

// Function to handle capturing (simulating saving) a photo
void capture_photo() {
    char photo_name[MAX_NAME];
    printf("Enter photo name: ");
    if (scanf("%s", photo_name) == 1) {
        ensure_directory_exists();

        // Construct full file path
        char filepath[MAX_NAME + 32];
        snprintf(filepath, sizeof(filepath), "%s/%s", DIRECTORY_NAME, photo_name);

        // Create an empty file to simulate saving the photo
        FILE *file = fopen(filepath, "w");
        if (file) {
            fprintf(file, "Simulated photo data for %s\n", photo_name);
            fclose(file);
            printf("Photo saved successfully!\n");
        } else {
            printf("Error: Could not save photo.\n");
        }
    }
}

// Function to view all photos in the single-level directory
void view_photos() {
    printf("\nCamera Directory:\n");
    
    DIR *dir = opendir(DIRECTORY_NAME);
    if (!dir) {
        // If directory doesn't exist yet, it's empty
        return;
    }

    struct dirent *entry;
    int found = 0;
    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden files like . and ..
        if (entry->d_name[0] != '.') {
            printf("%s\n", entry->d_name);
            found = 1;
        }
    }
    closedir(dir);
}

int main() {
    int choice;

    while (1) {
        printf("\n--- DIGITAL CAMERA ---\n");
        printf("1. Capture Photo\n");
        printf("2. View Photos\n");
        printf("3. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }

        switch (choice) {
            case 1:
                capture_photo();
                break;
            case 2:
                view_photos();
                break;
            case 3:
                printf("Exiting camera...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}