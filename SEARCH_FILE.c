#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void searchInFile(const char option, const char *filename, const char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char line[256];
    int count = 0;
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern)) {
            count++;
            if (option == 'f' && !found) {
                printf("First occurrence: %s", line);
                found = 1;
            }
            if (option == 'a') {
                printf("%s", line);
            }
        }
    }
    
    if (option == 'e') {
        printf("Number of occurrences: %d\n", count);
    }

    fclose(file);
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

        if (strcmp(args[0], "search") == 0 && args[1] && args[2] && args[3]) {
            searchInFile(args[1][0], args[2], args[3]);
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