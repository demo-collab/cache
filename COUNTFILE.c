#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void countFile(const char option, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    int characters = 0, words = 0, lines = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        characters++;
        if (ch == '\n') lines++;
        if (ch == ' ' || ch == '\n') words++;
    }
    if (characters > 0) lines++;
    if (characters > 0 && (ch != ' ' && ch != '\n')) words++;

    if (option == 'c') {
        printf("Number of characters: %d\n", characters);
    } else if (option == 'w') {
        printf("Number of words: %d\n", words);
    } else if (option == 'l') {
        printf("Number of lines: %d\n", lines);
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

        if (strcmp(args[0], "count") == 0 && args[1] && args[2]) {
            countFile(args[1][0], args[2]);
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