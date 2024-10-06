#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>

void listFiles(char option, const char *dirname) {
    struct dirent *entry;
    DIR *dp = opendir(dirname);
    if (!dp) {
        perror("Error opening directory");
        return;
    }
    int count = 0;

    while ((entry = readdir(dp)) != NULL) {
        if (option == 'f') {
            printf("%s\n", entry->d_name);
        } else if (option == 'n') {
            count++;
        } else if (option == 'i') {
            struct stat fileStat;
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirname, entry->d_name);
            if (stat(filepath, &fileStat) == 0) {
                printf("%s: %lu\n", entry->d_name, fileStat.st_ino);
            }
        }
    }
    if (option == 'n') {
        printf("Number of entries: %d\n", count);
    }
    closedir(dp);
}

int main() {
    char command[256];
    while (1) {
        printf("myshell$ ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0)
            break;

        char *args[10];
        int i = 0;
        char *token = strtok(command, " ");
        while (token) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "list") == 0 && args[1] && args[2]) {
            listFiles(args[1][0], args[2]);
        } else {
            pid_t pid = fork();
            if (pid == 0) {
                execvp(args[0], args);
                perror("Command execution failed");
                exit(1);
            } else {
                wait(NULL);
            }
        }
    }
    return 0;
}