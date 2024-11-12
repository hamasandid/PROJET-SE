#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 1024

typedef struct FileNode {
    char path[MAX_PATH_LENGTH];
    struct FileNode *next;
} FileNode;

FileNode *fileListHead = NULL;
pthread_mutex_t fileListMutex;

void add_file_to_list(const char *path) {
    pthread_mutex_lock(&fileListMutex);

    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    strncpy(newNode->path, path, MAX_PATH_LENGTH);
    newNode->next = fileListHead;
    fileListHead = newNode;

    pthread_mutex_unlock(&fileListMutex);
}

void *search_directory(void *arg) {
    char *dirPath = (char *)arg;
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("opendir");
        free(dirPath);
        return NULL;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char fullPath[MAX_PATH_LENGTH];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);

        if (entry->d_type == DT_DIR) {
            // Create a new thread for the subdirectory
            char *subDirPath = strdup(fullPath);
            pthread_t thread;
            pthread_create(&thread, NULL, search_directory, subDirPath);
            pthread_detach(thread);
        } else if (entry->d_type == DT_REG) {
            add_file_to_list(fullPath);
        }
    }

    closedir(dir);
    free(dirPath);
    return NULL;
}

void print_file_list() {
    pthread_mutex_lock(&fileListMutex);
    FileNode *current = fileListHead;
    while (current != NULL) {
        printf("%s\n", current->path);
        current = current->next;
    }
    pthread_mutex_unlock(&fileListMutex);
}

void free_file_list() {
    pthread_mutex_lock(&fileListMutex);
    FileNode *current = fileListHead;
    while (current != NULL) {
        FileNode *tmp = current;
        current = current->next;
        free(tmp);
    }
    fileListHead = NULL;
    pthread_mutex_unlock(&fileListMutex);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize the mutex
    pthread_mutex_init(&fileListMutex, NULL);

    // Start the search in the specified directory
    char *startDir = strdup(argv[1]);
    search_directory(startDir);

    // Wait for user to terminate (for simplicity in this example)
    sleep(1);

    // Print all found files
    printf("Files found:\n");
    print_file_list();

    // Clean up
    free_file_list();
    pthread_mutex_destroy(&fileListMutex);

    return 0;
}
