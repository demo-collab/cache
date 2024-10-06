#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void typeline(int n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char line[256];
    if (n > 0) {
        for (int i = 0; i < n && fgets(line, sizeof(line), file); i++)
            printf("%s", line);
    } else {
        while (fgets(line, sizeof(line), file))
            printf("%s", line);
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

        if (strcmp(args[0], "typeline") == 0) {
            if (strcmp(args[1], "-a") == 0 && args[2]) {
                typeline(0, args[2]);
            } else if (args[1] && sscanf(args[1], "+%d", &i) == 1 && args[2]) {
                typeline(i, args[2]);
            } else {
                printf("Invalid command usage.\n");
            }
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